#include <gtest/gtest.h>

#include "vm/ExecutionThread.h"
#include "Exception.h"

using namespace cish::vm;


class CounterThread: public ExecutionThread
{
public:
    typedef std::function<void()> Func;

    CounterThread(): CounterThread([](){}) {}
    CounterThread(Func func): _func(func)
    {
        _counter = 0;
    }

    ~CounterThread()
    {
        terminate();
    }

    int getCount() const
    {
        return _counter;
    }

protected:
    virtual void execute() override
    {
        while (true) {
            await();
            _func();
            _counter++;
        }
    }

private:
    Func _func;
    std::atomic_int _counter;
};


TEST(ExecutionThreadTest, terminationIdempotence)
{
    CounterThread thread;
    thread.startAsync();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    thread.terminate();
    thread.terminate();
    thread.terminate();
}

TEST(ExecutionThreadTest, exceptionsTerminateThread)
{
    CounterThread thread([]() { Throw(cish::Exception, "hei"); });
    thread.setWaitForResume(true);

    thread.startAsync();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    printf("Sending initial signal\n");
    ASSERT_TRUE(thread.isRunning());

    thread.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_FALSE(thread.isRunning());
}

TEST(ExecutionThreadTest, executionThreadWaitsForResume)
{
    CounterThread thread;
    thread.setWaitForResume(true);

    thread.startAsync();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_EQ(0, thread.getCount());

    for (int i=1; i<5; i++) {
        thread.resume();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ASSERT_EQ(i, thread.getCount());
    }
}

TEST(ExecutionThreadTest, terminationWorksWithRunningFree)
{
    CounterThread thread;
    thread.setWaitForResume(false);

    thread.startAsync();

    // By sleeping for 10ms we should have looped at least five times
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_LT(5, thread.getCount());
}

TEST(ExecutionThreadTest, resumeDoesNotBlock)
{
    CounterThread thread([](){ std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    thread.setWaitForResume(true);
    thread.startAsync();

    thread.resume();
    ASSERT_EQ(0, thread.getCount());
}


TEST(ExecutionThreadTest, cycleDoesBlock)
{
    CounterThread thread([](){ std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    thread.setWaitForResume(true);
    thread.startAsync();

    thread.cycle();
    ASSERT_EQ(1, thread.getCount());
}
