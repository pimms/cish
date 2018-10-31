#include <gtest/gtest.h>

#include "vm/ExecutionContext.h"


using namespace cish::vm;
using namespace cish::ast;


TEST(ExecutionContextTest, rootStackFrameExists)
{
    ExecutionContext context;
    ASSERT_NE(nullptr, context.getStackFrame());
}

TEST(ExecutionContextTest, poppingRootStackFrameThrows)
{
    ExecutionContext context;
    ASSERT_THROW(context.popStackFrame(), StackUnderflowException);

    context.pushStackFrame();
    ASSERT_NO_THROW(context.popStackFrame());
    ASSERT_THROW(context.popStackFrame(), StackUnderflowException);
}

TEST(ExecutionContextTest, excessivePushingEventuallyThrows)
{
    ExecutionContext context;

    ASSERT_THROW({
        // Here's to hoping this throws! *cheers*
        while (true) {
            context.pushStackFrame();
        }
    }, StackOverflowException);
}


