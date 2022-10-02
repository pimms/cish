#include "NegationExpression.h"

namespace cish::ast
{

NegationExpression::NegationExpression(Expression::Ptr expression):
    _expression(expression)
{ }

ExpressionValue NegationExpression::evaluate(vm::ExecutionContext *context) const
{
    ExpressionValue toNegate = _expression->evaluate(context);
    return !(toNegate.get<bool>());
}

TypeDecl NegationExpression::getType() const
{
    return TypeDecl::BOOL;
}

}