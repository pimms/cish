#include "ExecutionThread.h"

#include "../Exception.h"

#include <stdexcept>


namespace cish
{
namespace vm
{

struct TerminateSignal {
    std::string reason;
};


ExecutionThread::ExecutionThread():
    _isRunning(false),
    _execOrder(ExecOrder::RUNNING_FREE),
    _nextRequest(0),
    _lastHandled(0)
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

    _isRunning = true;

    _thread = std::thread([this]() {
        backgroundMain();
    });
}

void ExecutionThread::resume()
{
    std::lock_guard<std::mutex> lk(_mutex);
    _nextRequest++;
    _cvar.notify_all();
}

void ExecutionThread::terminate()
{
    if (_isRunning) {
        std::unique_lock<std::mutex> lock(_mutex);
        _execOrder = ExecOrder::TERMINATE;
        _cvar.notify_all();
        lock.unlock();

        if (_thread.joinable()) {
            _thread.join();
        }
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
        std::unique_lock<std::mutex> lk(_mutex);
        _cvar.wait(lk, [this, &state]() -> bool {
            state = getContinuationState();
            return state != ContinuationState::SLEEP;
        });

        _lastHandled.store(_nextRequest);
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
            return (_nextRequest > _lastHandled)
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
