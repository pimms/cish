#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"

#include "../Exception.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(FunctionNotDeclaredException);
DECLARE_EXCEPTION(InvalidParameterException);


class FunctionCallExpression: public Expression
{
public:
    FunctionCallExpression(DeclarationContext *context, const std::string &funName, std::vector<Expression*> params);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) override;

private:
    FuncDeclaration _funcDecl;
    std::vector<Expression*> _params;
};


}
}
