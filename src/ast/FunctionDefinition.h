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

    ExpressionValue execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const;

    bool hasReturned() const;
    ExpressionValue getReturnValue() const;

private:
    FuncDeclaration _decl;
};


}
}
