#include <gtest/gtest.h>

#include "vm/StackFrame.h"
#include "vm/Variable.h"
#include "vm/Memory.h"
#include "vm/Allocation.h"


using namespace cish::vm;


Variable* createVariable(Memory &mem, uint32_t len)
{
    auto alloc = mem.allocate(len);
    return new Variable(cish::ast::TypeDecl(cish::ast::TypeDecl::VOID), std::move(alloc));
}


TEST(StackFrameTest, UndefinedVariablesReturnsNull)
{
    StackFrame frame;
    ASSERT_EQ(nullptr, frame.getVariable("var"));
}

TEST(StackFrameTest, StoredVariablesAreRetrievable)
{
    Memory mem(100, 4);
    StackFrame frame;

    Variable *var = createVariable(mem, 4);
    frame.addVariable("var", var);

    ASSERT_NE(nullptr, frame.getVariable("var"));
    ASSERT_EQ(4, frame.getVariable("var")->getAllocation()->getSize());
}

TEST(StackFrameTest, ParentsVariablesAreAvailableInChildren)
{
    Memory mem(100, 4);
    StackFrame parent;
    StackFrame child(&parent);

    Variable *var = createVariable(mem, 4);
    parent.addVariable("var", var);

    ASSERT_NE(nullptr, child.getVariable("var"));
}

TEST(StackFrameTest, ChildVariablesOvershadowParents)
{
    Memory mem(100, 4);
    StackFrame parent;
    StackFrame child(&parent);

    Variable *v1 = createVariable(mem, 4);
    Variable *v2 = createVariable(mem, 8);

    parent.addVariable("var", v1);
    child.addVariable("var", v2);

    ASSERT_EQ(4, parent.getVariable("var")->getAllocation()->getSize());
    ASSERT_EQ(8, child.getVariable("var")->getAllocation()->getSize());
}



