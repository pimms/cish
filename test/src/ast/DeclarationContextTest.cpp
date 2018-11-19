#include <gtest/gtest.h>

#include "ast/DeclarationContext.h"
#include "ast/FunctionDefinition.h"


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
    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));
    context.enterFunction(&func);

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

    context.exitFunction();
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


TEST(DeclarationContextTest, canOnlyBeInOneFunctionScope)
{
    DeclarationContext context;
    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));

    ASSERT_NO_THROW(context.enterFunction(&func));
    ASSERT_THROW(context.enterFunction(&func), InvalidDeclarationScope);
}

TEST(DeclarationContextTest, excessiveFunctionPoppingThrows)
{
    DeclarationContext context;
    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));

    ASSERT_THROW(context.exitFunction(), InvalidDeclarationScope);

    ASSERT_NO_THROW(context.enterFunction(&func));
    ASSERT_NO_THROW(context.exitFunction());

    ASSERT_THROW(context.exitFunction(), InvalidDeclarationScope);
}

TEST(DeclarationContextTest, globalVariablesAreDeclaredInFunctions)
{
    DeclarationContext context;
    context.declareVariable(TypeDecl::INT, "global");

    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));
    context.enterFunction(&func);
    ASSERT_NE(nullptr, context.getVariableDeclaration("global"));
}

TEST(DeclarationContextTest, varScopesAreNotGloballyAvailable)
{
    DeclarationContext context;
    ASSERT_THROW(context.pushVariableScope(), InvalidDeclarationScope);
}

TEST(DeclarationContextTest, varScopesMustBeBalancedWithinFunction)
{
    DeclarationContext context;
    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));
    context.enterFunction(&func);
    context.pushVariableScope();
    context.pushVariableScope();
    context.popVariableScope();
    ASSERT_THROW(context.exitFunction(), InvalidDeclarationScope) << "Cannot exit with stacked scopes";
    context.popVariableScope();
    ASSERT_THROW(context.popVariableScope(), InvalidDeclarationScope) << "Cannot pop root scope";
    context.exitFunction();
}

TEST(DeclarationContextTest, functionRootVariablesAreNotGloballyDeclared)
{
    DeclarationContext context;

    FunctionDefinition func(&context, FuncDeclaration(TypeDecl::INT, "foo"));
    context.enterFunction(&func);
    context.declareVariable(TypeDecl::INT, "var");
    ASSERT_NE(nullptr, context.getVariableDeclaration("var"));
    context.exitFunction();
    ASSERT_EQ(nullptr, context.getVariableDeclaration("var"));
}

