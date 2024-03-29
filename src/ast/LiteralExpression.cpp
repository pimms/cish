#include "LiteralExpression.h"


namespace cish::ast
{

LiteralExpression::LiteralExpression(const std::string &literal): _value(literal) {}

LiteralExpression::LiteralExpression(ExpressionValue value): _value(value) {}

TypeDecl LiteralExpression::getType() const
{
    return _value.getIntrinsicType();
}

ExpressionValue LiteralExpression::evaluate(vm::ExecutionContext*) const
{
    return _value;
}

}
