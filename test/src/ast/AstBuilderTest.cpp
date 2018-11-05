#include <gtest/gtest.h>

#include "ast/AstBuilder.h"
#include "ast/AntlrContext.h"
#include "ast/LiteralExpression.h"
#include "ast/VariableDeclarationStatement.h"

using namespace cish::vm;
using namespace cish::ast;

Ast::Ptr buildAst(const std::string &source)
{
    AntlrContext context(source);

    AstBuilder builder(&context);
    return std::move(builder.buildAst());
}

TEST(AstBuilderTest, simpleGlobalVariable)
{
    Ast::Ptr ast = buildAst("int a = 15;");
    auto statements = ast->getRootStatements();
    ASSERT_EQ(1, statements.size());
    ASSERT_NE(nullptr, dynamic_cast<VariableDeclarationStatement*>(statements[0]));
}

TEST(AstBuilderTest, semiComplexGlobalVariables)
{
    Ast::Ptr ast = buildAst(
        "int a = 15; "
        "float b = 15.3; "
        "int c = a + b * 4;"
    );

    auto statements = ast->getRootStatements();
    ASSERT_EQ(3, statements.size());
    ASSERT_NE(nullptr, dynamic_cast<VariableDeclarationStatement*>(statements[0]));
    ASSERT_NE(nullptr, dynamic_cast<VariableDeclarationStatement*>(statements[1]));
    ASSERT_NE(nullptr, dynamic_cast<VariableDeclarationStatement*>(statements[2]));
}



TEST(AstBuilderTest, compilationPassingSmokeTest)
{
    // Not really checking anything of significance here, but
    // all of these programs should be AST'd correctly.
    EXPECT_NO_THROW(
        buildAst(
            "int global = 99; \n"
            "void foo(int x) { int y = x * 5 + global; } \n"
            "void bar() { foo(45); } \n"
        );
    );
}
