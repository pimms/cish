#include <gtest/gtest.h>

#include "vm/ExecutionContext.h"

#include "ast/LiteralExpression.h"


using namespace cish::vm;
using namespace cish::ast;


TEST(ExecutionContextTest, rootScopeExists)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);
    ASSERT_NE(nullptr, context.getScope());
}

TEST(ExecutionContextTest, pushingScopeOutsideFunctionThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);

    ASSERT_ANY_THROW(context.pushScope());
}

TEST(ExecutionContextTest, poppingScopeOutsideFunctionThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);

    ASSERT_ANY_THROW(context.popScope());
}

TEST(ExecutionContextTest, poppingRootScopeThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);
    context.pushFunctionFrame();

    ASSERT_ANY_THROW(context.popScope());

    ASSERT_NO_THROW(context.pushScope());
    ASSERT_NO_THROW(context.popScope());

    ASSERT_ANY_THROW(context.popScope());

    context.popFunctionFrame();
}

TEST(ExecutionContextTest, excessivePushingEventuallyThrows)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);

    ASSERT_THROW({
        // Here's to hoping this throws! *cheers*
        while (true) {
            context.pushFunctionFrame();
        }
    }, StackOverflowException);
}


TEST(ExecutionContextTest, functionFramesHidesNonGlobalScopes)
{
    Memory memory(100, 1);
    ExecutionContext context(&memory);

    Variable *global = new Variable(TypeDecl::INT, memory.allocate(4));
    context.getScope()->addVariable("global", global);
    ASSERT_EQ(global, context.getScope()->getVariable("global"));

    // Call a function
    Variable *inFirst = new Variable(TypeDecl::INT, memory.allocate(4));
    context.pushFunctionFrame();
    context.getScope()->addVariable("inFirst", inFirst);
    ASSERT_EQ(global, context.getScope()->getVariable("global"));
    ASSERT_EQ(inFirst, context.getScope()->getVariable("inFirst"));

    // Call a second function
    Variable *inSecond= new Variable(TypeDecl::INT, memory.allocate(4));
    context.pushFunctionFrame();
    context.getScope()->addVariable("inSecond", inSecond);
    ASSERT_EQ(global, context.getScope()->getVariable("global"));
    ASSERT_EQ(nullptr, context.getScope()->getVariable("inFirst"));
    ASSERT_EQ(inSecond, context.getScope()->getVariable("inSecond"));

    // Second function returns
    context.popFunctionFrame();
    ASSERT_EQ(global, context.getScope()->getVariable("global"));
    ASSERT_EQ(inFirst, context.getScope()->getVariable("inFirst"));
    ASSERT_EQ(nullptr, context.getScope()->getVariable("inSecond"));

    // First function returns
    context.popFunctionFrame();
    ASSERT_EQ(global, context.getScope()->getVariable("global"));
    ASSERT_EQ(nullptr, context.getScope()->getVariable("inFirst"));
    ASSERT_EQ(nullptr, context.getScope()->getVariable("inSecond"));

}


