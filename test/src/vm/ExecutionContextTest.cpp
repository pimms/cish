#include <gtest/gtest.h>

#include "vm/ExecutionContext.h"


using namespace cish::vm;
using namespace cish::ast;


TEST(ExecutionContextTest, rootScopeExists)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);
    ASSERT_NE(nullptr, context.getScope());
}

TEST(ExecutionContextTest, poppingRootScopeThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);
    ASSERT_THROW(context.popScope(), StackUnderflowException);

    context.pushScope();
    ASSERT_NO_THROW(context.popScope());
    ASSERT_THROW(context.popScope(), StackUnderflowException);
}

TEST(ExecutionContextTest, excessivePushingEventuallyThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);

    ASSERT_THROW({
        // Here's to hoping this throws! *cheers*
        while (true) {
            context.pushScope();
        }
    }, StackOverflowException);
}


