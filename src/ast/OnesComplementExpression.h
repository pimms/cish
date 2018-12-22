#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class OnesComplementExpression: public Expression
{
public:
    OnesComplementExpression(Expression::Ptr expression);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
	TypeDecl getType() const override;

private:
    Expression::Ptr _expression;
};


}
}
