#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class FunctionDefinition: public AstNode, public SuperStatement
{
public:
    FunctionDefinition(DeclarationContext *context,
                       FuncDeclaration decl,
                       StatementList statements);
    ~FunctionDefinition();

    const FuncDeclaration* getDeclaration() const;

private:
    FuncDeclaration _decl;
};


}
}
