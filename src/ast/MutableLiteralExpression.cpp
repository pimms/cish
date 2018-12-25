#include "MutableLiteralExpression.h"


namespace cish
{
namespace ast
{


MutableLiteralExpression::MutableLiteralExpression(const std::string &literal): _value(literal) {}

MutableLiteralExpression::MutableLiteralExpression(ExpressionValue value): _value(value) {}

TypeDecl MutableLiteralExpression::getType() const
{
    return _value.getIntrinsicType();
}

ExpressionValue MutableLiteralExpression::evaluate(vm::ExecutionContext*) const
{
    return _value;
}

void MutableLiteralExpression::setValue(const ExpressionValue &newValue)
{
    if (newValue.getIntrinsicType() != _value.getIntrinsicType()) {
        Throw(Exception, "Cannot change value to value of different type (has %s, got %s)",
                _value.getIntrinsicType().getName(),
                newValue.getIntrinsicType().getName());
    }

    _value = newValue;
}


}
}
