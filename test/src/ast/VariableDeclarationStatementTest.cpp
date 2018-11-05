#include <gtest/gtest.h>

#include "ast/VariableDeclarationStatement.h"
#include "vm/Memory.h"
#include "vm/ExecutionContext.h"


using namespace cish::ast;
using namespace cish::vm;


TEST(VariableDeclarationStatementTest, variablesAreDeclaredInDeclarationContext)
{
    DeclarationContext dc;
    ASSERT_EQ(nullptr, dc.getVariableDeclaration("var"));

    VariableDeclarationStatement stmt(&dc, TypeDecl::INT, "var");

    auto var = dc.getVariableDeclaration("var");
    ASSERT_NE(nullptr, var);
    ASSERT_EQ("var", var->name);
    ASSERT_EQ(TypeDecl(TypeDecl::INT), var->type);
}

TEST(VariableDeclarationStatementTest, variablesAreDeclaredInExecutionContext)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    VariableDeclarationStatement stmt(&dc, TypeDecl::INT, "var");

    ASSERT_EQ(nullptr, ec.getStackFrame()->getVariable("var"));
    stmt.execute(&ec);

    Variable *var = ec.getStackFrame()->getVariable("var");
    ASSERT_NE(nullptr, var);
    ASSERT_EQ(TypeDecl(TypeDecl::INT), var->getType());
}

TEST(VariableDeclarationStatementTest, variablesAreInitializedWhenExpressionIsDefined)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    auto expr = new LiteralExpression(100);

    VariableDeclarationStatement stmt(&dc, TypeDecl::INT, "var", expr);
    stmt.execute(&ec);

    Variable *var = ec.getStackFrame()->getVariable("var");
    ASSERT_EQ(100, var->getAllocation()->read<int>());
}