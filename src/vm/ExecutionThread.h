#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

#include "../Exception.h"

namespace cish::vm::internal
{

class Signal
{
public:
    Signal();

    void wait(std::function<bool()> cond);
    void signal(std::function<void()> prenotif);

private:
    std::mutex _mutex;
    std::condition_variable _cvar;
};

}

namespace cish::vm
{

class ExecutionThread
{
public:
    ExecutionThread();
    virtual ~ExecutionThread();

    void setWaitForResume(bool waitForResume);

    void startAsync();
    void runBlocking();

    /**
     * Allow the background thread to continue. Has no effect if
     * 'waitForResume' is false or the worker isn't waiting for
     * a notification from the organizer thread.
     */
    void resume();

    /**
     * Signal the background thread to continue, and also wait for
     * it to wait for another instruction - effectively waiting a
     * single cycle.
     *
     * A deadlock condition WILL happen if the worker thread never
     * calls 'await()' again.
     */
    void cycle();

    /**
     * Terminate the background thread. The worker thread will
     * attempt to terminate its' efforts on the next call to 'await()'.
     */
    void terminate();

    bool isRunning() const;

    std::shared_ptr<Exception> getRuntimeError() const;

protected:
    // Must be called from the worker thread only!
    void await();
    virtual void execute() = 0;

private:
    enum class ExecOrder
    {
        WAIT_FOR_RESUME,
        RUNNING_FREE,
        TERMINATE,
    };

    enum class ContinuationState
    {
        SLEEP,
        CONTINUE,
        TERMINATE,
    };

    void start(bool waitForCompletion);

    // Called in the std::condition_variable callback to
    // handle spurious wakeups
    ContinuationState getContinuationState() const;
    void backgroundMain();

    std::atomic_bool _isRunning;

    std::thread _thread;
    internal::Signal _orgToWorker;
    internal::Signal _workerToOrg;

    std::atomic<ExecOrder> _execOrder;
    std::atomic_long _nextRequest;
    std::atomic_long _lastRequestReceived;
    std::atomic_long _lastRequestHandled;

    std::shared_ptr<Exception> _runtimeError;
};

}
