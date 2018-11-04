#include <gtest/gtest.h>

#include "ast/DeclarationContext.h"


using namespace cish::ast;


TEST(DeclarationContextTest, undeclaredVariablesReturnsNull)
{
    DeclarationContext context;

    context.declareVariable(TypeDecl::INT, "foo");
    ASSERT_EQ(nullptr, context.getVariableDeclaration("var"));
}

TEST(DeclarationContextTest, declaredVariablesReturnsNonNull)
{
    DeclarationContext context;

    context.declareVariable(TypeDecl::INT, "var");
    ASSERT_NE(nullptr, context.getVariableDeclaration("var"));
}

TEST(DeclarationContextTest, declaringAlreadyDeclaredVariableThrows)
{
    DeclarationContext context;
    context.declareVariable(TypeDecl::INT, "var");

    ASSERT_THROW(context.declareVariable(TypeDecl::FLOAT, "var"), VariableAlreadyDeclaredException);
}

TEST(DeclarationContext, shadowingAllowedInNewScopes)
{
    DeclarationContext context;

    // At first it's an int
    context.declareVariable(TypeDecl::INT, "var");
    ASSERT_EQ(TypeDecl::INT, context.getVariableDeclaration("var")->type.getType());

    // Then it's a float
    context.pushVariableScope();
    context.declareVariable(TypeDecl::FLOAT, "var");
    ASSERT_EQ(TypeDecl::FLOAT, context.getVariableDeclaration("var")->type.getType());

    // Pop it, and it's once again an int
    context.popVariableScope();
    ASSERT_EQ(TypeDecl::INT, context.getVariableDeclaration("var")->type.getType());
}

