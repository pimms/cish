#include "ExecutionThread.h"

#include "../Exception.h"

#include <stdexcept>


namespace cish::vm::internal
{

// #define ENABLE_DBGLOG
#ifdef ENABLE_DBGLOG
    #define DBGLOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define DBGLOG(...) do{}while(0);
#endif


Signal::Signal()
{

}

void Signal::wait(std::function<bool()> cond)
{
    std::unique_lock<std::mutex> lk(_mutex);
    _cvar.wait(lk, cond);
}

void Signal::signal(std::function<void()> prenotif)
{
    std::lock_guard<std::mutex> lk(_mutex);
    prenotif();
    _cvar.notify_all();
}

}

namespace cish::vm
{

struct TerminateSignal {
    std::string reason;
};

ExecutionThread::ExecutionThread():
    _isRunning(false),
    _execOrder(ExecOrder::RUNNING_FREE),
    _nextRequest(0),
    _lastRequestReceived(0),
    _lastRequestHandled(0)
{

}

ExecutionThread::~ExecutionThread()
{
    // If the background thread is running at this point, there's
    // a VERY potent race-condition in the virtual 'execute()' method
    // being called after the subclass of ExecutionThread has been
    // destroyed, meaning that we'd crash at runtime due to a pure
    // virtual function call. FAIL FAST.
    //
    // Note that when I've tried to provoke this race condition, it
    // has typically taken around 10k attempts to crash. This is still
    // too frequent. Call terminate() from the subclass' dtor!
    assert(!_isRunning);

    // This call *should* be redundant, but just in case Jimmy compiles
    // without asserts we should almost always be safe.
    terminate();

    // If the background thread crashed, we'd never call join.
    // Call it once more to be safe.
    if (_thread.joinable()) {
        _thread.join();
    }
}

void ExecutionThread::setWaitForResume(bool waitForResume)
{
    if (_execOrder != ExecOrder::TERMINATE) {
        _execOrder = (waitForResume ? ExecOrder::WAIT_FOR_RESUME : ExecOrder::RUNNING_FREE);
    }
}

void ExecutionThread::startAsync()
{
    start(false);
}

void ExecutionThread::runBlocking()
{
    start(true);
}

void ExecutionThread::resume()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            _nextRequest++;
            DBGLOG("[O] resume/signaling next request (%d)\n", (int)_nextRequest.load());
        });
    }
}

void ExecutionThread::cycle()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            _nextRequest++;
            DBGLOG("[O] cycle/signaling next request (%d)\n", (int)_nextRequest.load());
        });

        _workerToOrg.wait([this]() {
            return _lastRequestHandled == _nextRequest || !_isRunning;
        });

#ifdef ENABLE_DBGLOG
        if (_lastRequestHandled == _nextRequest) {
            DBGLOG("[O] cycle/received process ack (%d)\n", (int)_nextRequest.load());
        } else {
            DBGLOG("[O] thread died instead of handling request\n");
        }
#endif
    }
}

void ExecutionThread::terminate()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            DBGLOG("[O] sending termination request\n");
            _execOrder = ExecOrder::TERMINATE;
        });

        DBGLOG("[O] joining...\n");
        if (_thread.joinable()) {
            _thread.join();
        }
        DBGLOG("[O] joined\n");
    }
}

bool ExecutionThread::isRunning() const
{
    return _isRunning;
}

std::shared_ptr<Exception> ExecutionThread::getRuntimeError() const
{
    return _runtimeError;
}

void ExecutionThread::await()
{
    // Keep CONTINUE as the default value, in case the spurious callback
    // is never called
    ContinuationState state = ContinuationState::CONTINUE;

    if (_execOrder == ExecOrder::WAIT_FOR_RESUME) {
        DBGLOG("[W] sending process ack (%d)\n", (int)_lastRequestReceived.load());
        _workerToOrg.signal([this]() {
            _lastRequestHandled.store(_lastRequestReceived);
        });

        DBGLOG("[W] awaiting process request\n");
        _orgToWorker.wait([this, &state]() -> bool {
            state = getContinuationState();
            return state != ContinuationState::SLEEP;
        });
        DBGLOG("[W] received worker signal\n");

        _lastRequestReceived.store(_nextRequest);
    } else {
        state = getContinuationState();
    }

    if (state == ContinuationState::TERMINATE) {
        throw TerminateSignal { "Received termination signal" };
    }
}

void ExecutionThread::start(bool waitForCompletion)
{
    if (_isRunning) {
        throw std::runtime_error("ExecutionThread has already been started");
    }

    std::mutex mutex;
    std::condition_variable var;
    std::atomic_bool started = false;
    std::atomic_bool done = false;

    _thread = std::thread([waitForCompletion,this,&var,&started,&done,&mutex]() {
        {
            std::lock_guard<std::mutex> lock(mutex);
        }

        _isRunning = true;
        started = true;
        var.notify_one();

        DBGLOG("[W] thread started\n");
        backgroundMain();

        if (waitForCompletion) {
            {
                DBGLOG("[W] signalling waiting org-thread\n");
                std::lock_guard<std::mutex> lock(mutex);
                done = true;
            }
            var.notify_one();
        }

        DBGLOG("[W] -- thread terminated --\n");
    });

    {
        std::unique_lock<std::mutex> lock(mutex);
        var.wait(lock, [&]() { return started.load(); });
        DBGLOG("[O] background thread started\n");
    }

    if (waitForCompletion) {
        DBGLOG("[O] waiting for worker to complete\n");
        std::unique_lock<std::mutex> lock(mutex);
        var.wait(lock, [&]() { return done.load(); });
        DBGLOG("[O] worker completed (isrunning=%d)\n", _isRunning.load());
        _thread.join();
        DBGLOG("[O] joined\n");
    }
}


ExecutionThread::ContinuationState ExecutionThread::getContinuationState() const
{
    ExecOrder order = _execOrder.load();
    switch (order) {
        case ExecOrder::WAIT_FOR_RESUME:
            return (_nextRequest > _lastRequestHandled)
                        ? ContinuationState::CONTINUE
                        : ContinuationState::SLEEP;
        case ExecOrder::RUNNING_FREE:
            return ContinuationState::CONTINUE;
        case ExecOrder::TERMINATE:
            return ContinuationState::TERMINATE;
    }
}

void ExecutionThread::backgroundMain()
{
    try {
        execute();
        DBGLOG("[W] Execution thread exiting normally\n");
    } catch (TerminateSignal tsig) {
        DBGLOG("[W] executionthread terminated\n");
    } catch (const Exception &e) {
        DBGLOG("[W] cish::Exception caught:\n%s\n", e.what());
        Exception *copy = new Exception(e);
        _runtimeError = std::shared_ptr<Exception>(copy);
    } catch (std::exception e) {
        DBGLOG("[W] std::exception caught:\n%s\n", e.what());
    } catch (...) {
        DBGLOG("[W] unknown throwable caught\n");
    }

    // If we are being called in a synchronous manner, the worker thread
    // will never send the final ack, so we need to do it manually.
    _isRunning = false;
    _workerToOrg.signal([this]() {
        DBGLOG("[W] signalling last request received\n");
        _lastRequestHandled.store(_lastRequestReceived);
    });
}

}
