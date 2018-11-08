#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class FunctionDefinition: public SuperStatement
{
public:
    FunctionDefinition(DeclarationContext *context, FuncDeclaration decl);
    ~FunctionDefinition();

    const FuncDeclaration* getDeclaration() const;

    void execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const;

private:
    FuncDeclaration _decl;
};


}
}
