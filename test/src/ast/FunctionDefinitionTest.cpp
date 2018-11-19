#include <gtest/gtest.h>

#include "ast/FunctionDefinition.h"
#include "ast/DeclarationContext.h"

using namespace cish::ast;


TEST(FunctionDefinitionTest, definitionImplicitlyDeclares)
{
    DeclarationContext dc;

    FuncDeclaration decl;
    decl.returnType = TypeDecl::INT;
    decl.name = "main";

    FunctionDefinition def(&dc, decl);

    ASSERT_NE(nullptr, dc.getFunctionDeclaration("main"));
}

