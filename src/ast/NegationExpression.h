#pragma once

#include "AstNodes.h"

namespace cish::ast
{

class NegationExpression: public Expression
{
public:
    NegationExpression(Expression::Ptr expression);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
	TypeDecl getType() const override;

private:
    Expression::Ptr _expression;
};

}
