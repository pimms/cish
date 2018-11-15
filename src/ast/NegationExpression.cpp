#include "NegationExpression.h"


namespace cish
{
namespace ast
{


NegationExpression::NegationExpression(Expression *expression):
    _expression(expression)
{ }

NegationExpression::~NegationExpression()
{
    delete _expression;
}

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
}