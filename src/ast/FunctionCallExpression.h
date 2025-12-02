#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"

#include "../Exception.h"

namespace cish::ast
{

DECLARE_EXCEPTION(FunctionNotDeclaredException);
DECLARE_EXCEPTION(InvalidParameterException);


class FunctionCallExpression: public Expression
{
public:
    FunctionCallExpression(DeclarationContext *context,
            const std::string &funName,
            std::vector<Expression::Ptr> params);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    void verifyParameterTypes();

    FuncDeclaration _funcDecl;
    std::vector<Expression::Ptr> _params;
};

}
