#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class NegationExpression: public Expression
{
public:
    NegationExpression(Expression *expression);
    ~NegationExpression();

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
	TypeDecl getType() const override;

private:
    Expression *_expression;
};


}
}