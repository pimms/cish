#include <gtest/gtest.h>


#include "ast/SuperStatement.h"
#include "ast/VariableDeclarationStatement.h"
#include "ast/Type.h"
#include "ast/DeclarationContext.h"

#include "vm/Memory.h"
#include "vm/ExecutionContext.h"


using namespace cish::vm;
using namespace cish::ast;

TEST(SuperStatementTest, superStatementsCallAllTheirChildren)
{
    Memory memory(100, 1);
    ExecutionContext ec(&memory);
    DeclarationContext dc;

    auto *decl1 = new VariableDeclarationStatement(&dc, TypeDecl::INT, "var1", nullptr);
    auto *decl2 = new VariableDeclarationStatement(&dc, TypeDecl::SHORT, "var2", nullptr);

    SuperStatement super;
    super.addStatement(decl1);
    super.addStatement(decl2);
    super.execute(&ec);

    ASSERT_NE(nullptr, ec.getScope()->getVariable("var1"));
    ASSERT_EQ(TypeDecl::INT, ec.getScope()->getVariable("var1")->getType().getType());

    ASSERT_NE(nullptr, ec.getScope()->getVariable("var2"));
    ASSERT_EQ(TypeDecl::SHORT, ec.getScope()->getVariable("var2")->getType().getType());

    ASSERT_EQ(100 - 6, memory.getFreeSize());
}

