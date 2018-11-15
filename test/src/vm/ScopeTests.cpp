#include <gtest/gtest.h>

#include "vm/Scope.h"
#include "vm/Variable.h"
#include "vm/Memory.h"
#include "vm/Allocation.h"


using namespace cish::vm;


Variable* createVariable(Memory &mem, uint32_t len)
{
    auto alloc = mem.allocate(len);
    return new Variable(cish::ast::TypeDecl(cish::ast::TypeDecl::VOID), std::move(alloc));
}


TEST(ScopeTest, UndefinedVariablesReturnsNull)
{
    Scope frame;
    ASSERT_EQ(nullptr, frame.getVariable("var"));
}

TEST(ScopeTest, StoredVariablesAreRetrievable)
{
    Memory mem(100, 4);
    Scope frame;

    Variable *var = createVariable(mem, 4);
    frame.addVariable("var", var);

    ASSERT_NE(nullptr, frame.getVariable("var"));
}

TEST(ScopeTest, ParentsVariablesAreAvailableInChildren)
{
    Memory mem(100, 4);
    Scope parent;
    Scope child(&parent);

    Variable *var = createVariable(mem, 4);
    parent.addVariable("var", var);

    ASSERT_NE(nullptr, child.getVariable("var"));
}

TEST(ScopeTest, ChildVariablesOvershadowParents)
{
    Memory mem(100, 4);
    Scope parent;
    Scope child(&parent);

    Variable *v1 = createVariable(mem, 4);
    Variable *v2 = createVariable(mem, 8);

    parent.addVariable("var", v1);
    child.addVariable("var", v2);
}



