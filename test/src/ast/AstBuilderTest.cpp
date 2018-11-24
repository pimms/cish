#include <gtest/gtest.h>

#include "module/ModuleContext.h"

#include "ast/AstBuilder.h"
#include "ast/AntlrContext.h"
#include "ast/LiteralExpression.h"
#include "ast/VariableDeclarationStatement.h"

using namespace cish::vm;
using namespace cish::ast;
using namespace cish::module;

Ast::Ptr buildAst(const std::string &source)
{
    ParseContext::Ptr parseContext = ParseContext::parseSource(source);
    AstBuilder builder(parseContext, ModuleContext::create());
    return std::move(builder.buildAst());
}

TEST(AstBuilderTest, simpleGlobalVariable)
{
    Ast::Ptr ast = buildAst("int a = 15;");
    auto statements = ast->getRootStatements();
    ASSERT_EQ(1, statements.size());
    ASSERT_NE(nullptr, dynamic_cast<const VariableDeclarationStatement*>(statements[0].get()));
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
    ASSERT_NE(nullptr, dynamic_cast<const VariableDeclarationStatement*>(statements[0].get()));
    ASSERT_NE(nullptr, dynamic_cast<const VariableDeclarationStatement*>(statements[1].get()));
    ASSERT_NE(nullptr, dynamic_cast<const VariableDeclarationStatement*>(statements[2].get()));
}

TEST(AstBuilderTest, pointerDeclaration)
{
    Ast::Ptr ast = buildAst("int* ptr;");
    auto statements = ast->getRootStatements();

    auto *stmt = dynamic_cast<const VariableDeclarationStatement*>(statements[0].get());
    ASSERT_NE(nullptr, stmt);

    TypeDecl type = stmt->getDeclaredType();
    ASSERT_EQ(TypeDecl::POINTER, type.getType());
    ASSERT_EQ(TypeDecl::INT, type.getReferencedType()->getType());
}

TEST(AstBuilderTest, pointerPointerDeclaration)
{
    Ast::Ptr ast = buildAst("int*  *   ptr;");
    auto statements = ast->getRootStatements();

    auto *stmt = dynamic_cast<const VariableDeclarationStatement*>(statements[0].get());
    ASSERT_NE(nullptr, stmt);

    TypeDecl type = stmt->getDeclaredType();
    ASSERT_EQ(TypeDecl::POINTER, type.getType());
    ASSERT_EQ(TypeDecl::POINTER, type.getReferencedType()->getType());
    ASSERT_EQ(TypeDecl::INT, type.getReferencedType()->getReferencedType()->getType());
}

TEST(AstBuilderTest, constVariableDeclaration)
{
    Ast::Ptr ast = buildAst("const int var = 15;");
    auto statements = ast->getRootStatements();
    auto *stmt = dynamic_cast<const VariableDeclarationStatement*>(statements[0].get());

    TypeDecl type = stmt->getDeclaredType();
    ASSERT_EQ(TypeDecl::INT, type.getType());
    ASSERT_TRUE(type.isConst());
}

TEST(AstBuilderTest, constPointerDeclaration)
{
    Ast::Ptr ast = buildAst("const int* var = 15;");
    auto statements = ast->getRootStatements();
    auto *stmt = dynamic_cast<const VariableDeclarationStatement*>(statements[0].get());

    TypeDecl type = stmt->getDeclaredType();
    ASSERT_EQ(TypeDecl::POINTER, type.getType());
    ASSERT_FALSE(type.isConst());
    ASSERT_EQ(TypeDecl::INT, type.getReferencedType()->getType());
    ASSERT_TRUE(type.getReferencedType()->isConst());
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
            "int var = global++;"
        );
    );
}
