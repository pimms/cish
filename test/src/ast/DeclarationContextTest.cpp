#include <gtest/gtest.h>

#include "ast/DeclarationContext.h"


using namespace cish::ast;


TEST(DeclarationContextTest, undeclaredVariablesReturnsNull)
{
    DeclarationContext context;

    context.declareVariable("foo", TypeDecl::INT);
    ASSERT_EQ(nullptr, context.getVariableDeclaration("var"));
}

TEST(DeclarationContextTest, declaredVariablesReturnsNonNull)
{
    DeclarationContext context;

    context.declareVariable("var", TypeDecl::INT);
    ASSERT_NE(nullptr, context.getVariableDeclaration("var"));
}

TEST(DeclarationContextTest, declaringAlreadyDeclaredVariableThrows)
{
    DeclarationContext context;
    context.declareVariable("var", TypeDecl::INT);

    ASSERT_THROW(context.declareVariable("var", TypeDecl::FLOAT), VariableAlreadyDeclaredException);
}

TEST(DeclarationContext, shadowingAllowedInNewScopes)
{
    DeclarationContext context;

    // At first it's an int
    context.declareVariable("var", TypeDecl::INT);
    ASSERT_EQ(TypeDecl::INT, context.getVariableDeclaration("var")->type.getType());

    // Then it's a float
    context.pushVariableScope();
    context.declareVariable("var", TypeDecl::FLOAT);
    ASSERT_EQ(TypeDecl::FLOAT, context.getVariableDeclaration("var")->type.getType());

    // Pop it, and it's once again an int
    context.popVariableScope();
    ASSERT_EQ(TypeDecl::INT, context.getVariableDeclaration("var")->type.getType());
}

