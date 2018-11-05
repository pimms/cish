#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"


namespace cish
{
namespace ast
{


class FunctionDefinition: public AstNode
{
public:
    FunctionDefinition(DeclarationContext *context,
                       FuncDeclaration decl,
                       std::vector<Statement*> statements);
    ~FunctionDefinition();

private:
    FuncDeclaration _decl;
    std::vector<Statement*> _statements;
};


}
}
