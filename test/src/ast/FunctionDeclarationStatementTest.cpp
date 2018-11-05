#include <gtest/gtest.h>

#include "ast/FunctionDeclarationStatement.h"
#include "ast/DeclarationContext.h"

using namespace cish::vm;
using namespace cish::ast;


TEST(FunctionDeclarationStatementTest, functionsAreDeclaredInContext)
{
    FuncDeclaration decl;
    decl.name = "foo";
    decl.returnType = TypeDecl::VOID;
    decl.params = { VarDeclaration { TypeDecl::INT, "n" } };

    DeclarationContext context;
    ASSERT_EQ(nullptr, context.getFunctionDeclaration("foo"));

    FunctionDeclarationStatement stmt(&context, decl);
    ASSERT_NE(nullptr, context.getFunctionDeclaration("foo"));
    ASSERT_EQ(decl.name, context.getFunctionDeclaration("foo")->name);
    ASSERT_EQ(decl.returnType, context.getFunctionDeclaration("foo")->returnType);
}

