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

TEST(DeclarationContextTest, shadowingAllowedInNewScopes)
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


TEST(DeclarationContextTest, redeclarationWithIdenticalSignatureIsAllowed)
{
    FuncDeclaration decl;
    decl.name = "foo";
    decl.returnType = TypeDecl::INT;
    decl.params = { VarDeclaration { TypeDecl::FLOAT, "x" } };

    DeclarationContext context;
    ASSERT_EQ(nullptr, context.getFunctionDeclaration("foo"));

    context.declareFunction(decl);
    ASSERT_NE(nullptr, context.getFunctionDeclaration("foo"));
    ASSERT_EQ(decl.name, context.getFunctionDeclaration("foo")->name);
    ASSERT_EQ(decl.returnType, context.getFunctionDeclaration("foo")->returnType);

    context.declareFunction(decl);
    ASSERT_NE(nullptr, context.getFunctionDeclaration("foo"));
    ASSERT_EQ(decl.name, context.getFunctionDeclaration("foo")->name);
    ASSERT_EQ(decl.returnType, context.getFunctionDeclaration("foo")->returnType);
}

TEST(DeclarationContextTest, redeclarationWithDifferentSignatureThrows)
{
    FuncDeclaration original;
    original.name = "foo";
    original.returnType = TypeDecl::INT;
    original.params = { VarDeclaration { TypeDecl::FLOAT, "x" } };

    DeclarationContext context;
    context.declareFunction(original);

    {
        FuncDeclaration redeclAsFloat = original;
        redeclAsFloat.returnType = TypeDecl::FLOAT;
        ASSERT_ANY_THROW(context.declareFunction(redeclAsFloat));
    }

    {
        FuncDeclaration redeclWithoutParams = original;
        redeclWithoutParams.params.clear();
        ASSERT_ANY_THROW(context.declareFunction(redeclWithoutParams));
    }

    {
        FuncDeclaration redeclWithExtraParams = original;
        redeclWithExtraParams.params.push_back(VarDeclaration {TypeDecl::INT, "n"});
        ASSERT_ANY_THROW(context.declareFunction(redeclWithExtraParams));
    }
}
