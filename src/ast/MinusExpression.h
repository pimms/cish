#pragma once

#include "AstNodes.h"

namespace cish::ast
{

class MinusExpression: public Expression
{
public:
    MinusExpression(Expression::Ptr expr);

    TypeDecl getType() const override;
    ExpressionValue evaluate(vm::ExecutionContext *ctx) const override;

private:
    Expression::Ptr _expr;
};

}
