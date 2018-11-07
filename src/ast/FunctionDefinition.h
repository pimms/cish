#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(InvalidParameterException);


class FunctionDefinition: public SuperStatement
{
public:
    FunctionDefinition(DeclarationContext *context, FuncDeclaration decl);
    ~FunctionDefinition();

    const FuncDeclaration* getDeclaration() const;

    void execute(vm::ExecutionContext *context, std::vector<ExpressionValue> params);

private:
    FuncDeclaration _decl;
};


}
}
