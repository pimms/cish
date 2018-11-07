#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

namespace cish
{
namespace vm
{


class ExecutionThread
{
public:
    ExecutionThread();
    ~ExecutionThread();

    void setWaitForResume(bool waitForResume);

    void start();
    void resume();
    void terminate();

    bool isRunning() const;

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

    // Called in the std::condition_variable callback to
    // handle spurious wakeups
    ContinuationState getContinuationState() const;
    void backgroundMain();

    bool _isRunning;

    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cvar;

    std::atomic<ExecOrder> _execOrder;
    std::atomic_long _nextRequest;
    std::atomic_long _lastHandled;
};


}
}
