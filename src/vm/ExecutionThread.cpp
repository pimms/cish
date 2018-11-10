#include "ExecutionThread.h"

#include "../Exception.h"

#include <stdexcept>


namespace cish
{
namespace vm
{

namespace internal
{

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

void ExecutionThread::start()
{
    if (_isRunning) {
        throw std::runtime_error("ExecutionThread has already been started");
    }


    std::mutex mutex;
    std::condition_variable var;
    std::atomic_bool ready = false;

    _thread = std::thread([&]() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            ready = true;
            var.notify_one();
        }

        // printf("[W] thread started\n");
        backgroundMain();
    });

    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return ready.load(); });
    _isRunning = true;
}

void ExecutionThread::resume()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            _nextRequest++;
            // printf("[O] resume/signaling next request (%d)\n", (int)_nextRequest.load());
        });
    }
}

void ExecutionThread::cycle()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            _nextRequest++;
            // printf("[O] cycle/signaling next request (%d)\n", (int)_nextRequest.load());
        });

        _workerToOrg.wait([this]() {
            return _lastRequestHandled == _nextRequest;
        });
        // printf("[O] cycle/received process ack (%d)\n", (int)_nextRequest.load());
    }
}

void ExecutionThread::terminate()
{
    if (_isRunning) {
        _orgToWorker.signal([this]() {
            // printf("[O] sending termination request\n");
            _execOrder = ExecOrder::TERMINATE;
        });

        // printf("[O] joining...\n");
        if (_thread.joinable()) {
            _thread.join();
        }
        // printf("[O] joined\n");
    }
}

bool ExecutionThread::isRunning() const
{
    return _isRunning;
}

void ExecutionThread::await()
{
    // Keep CONTINUE as the default value, in case the spurious callback
    // is never called
    ContinuationState state = ContinuationState::CONTINUE;

    if (_execOrder == ExecOrder::WAIT_FOR_RESUME) {
        // printf("[W] sending process ack (%d)\n", (int)_lastRequestReceived.load());
        _workerToOrg.signal([this]() {
            _lastRequestHandled.store(_lastRequestReceived);
        });

        // printf("[W] awaiting process request\n");
        _orgToWorker.wait([this, &state]() -> bool {
            state = getContinuationState();
            return state != ContinuationState::SLEEP;
        });
        // printf("[W] received worker signal\n");

        _lastRequestReceived.store(_nextRequest);
    } else {
        state = getContinuationState();
    }

    if (state == ContinuationState::TERMINATE) {
        throw TerminateSignal { "Received termination signal" };
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

        // If we are being called in a synchronous manner, the worker thread
        // will never send the final ack, so we need to do it manually.
        _workerToOrg.signal([this]() {
            _lastRequestHandled.store(_lastRequestReceived);
        });

        printf("Execution thread exiting normally\n");
    } catch (TerminateSignal tsig) {
        printf("[W] executionthread terminated\n");
    } catch (cish::Exception e) {
        printf("[W] cish::Exception caught:\n%s\n", e.what());
    } catch (std::exception e) {
        printf("[W] std::exception caught:\n%s\n", e.what());
    } catch (...) {
        printf("[W] unknown throwable caught\n");
    }


    _isRunning = false;
}


}
}
