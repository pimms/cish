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

    auto decl1 = std::make_shared<VariableDeclarationStatement>(&dc, TypeDecl::INT, "var1", nullptr);
    auto decl2 = std::make_shared<VariableDeclarationStatement>(&dc, TypeDecl::SHORT, "var2", nullptr);

    SuperStatement super;
    super.addStatement(std::dynamic_pointer_cast<Statement>(decl1));
    super.addStatement(std::dynamic_pointer_cast<Statement>(decl2));
    super.executeChildStatements(&ec);

    ASSERT_NE(nullptr, ec.getScope()->getVariable("var1"));
    ASSERT_EQ(TypeDecl::INT, ec.getScope()->getVariable("var1")->getType().getType());

    ASSERT_NE(nullptr, ec.getScope()->getVariable("var2"));
    ASSERT_EQ(TypeDecl::SHORT, ec.getScope()->getVariable("var2")->getType().getType());

    ASSERT_EQ(100 - 6, memory.getFreeSize());
}

