#include "MinusExpression.h"


namespace cish
{
namespace ast
{


MinusExpression::MinusExpression(Expression::Ptr expr):
    _expr(expr)
{
    if (expr->getType() == TypeDecl::POINTER) {
        Throw(InvalidTypeException, "Operator '-' cannot be applied to pointers");
    }
}

TypeDecl MinusExpression::getType() const
{
    return _expr->getType();
}

ExpressionValue MinusExpression::evaluate(vm::ExecutionContext *ctx) const
{
    ExpressionValue value = _expr->evaluate(ctx);
    const TypeDecl type = value.getIntrinsicType();

    switch (value.getIntrinsicType().getType()) {
        case TypeDecl::INT:
            return ExpressionValue(type, -value.get<int32_t>());
        case TypeDecl::FLOAT:
            return ExpressionValue(type, -value.get<float>());
        case TypeDecl::DOUBLE:
            return ExpressionValue(type, -value.get<double>());
        case TypeDecl::CHAR:
            return ExpressionValue(type, -value.get<int8_t>());
        case TypeDecl::SHORT:
            return ExpressionValue(type, -value.get<int16_t>());
        case TypeDecl::BOOL:
            return ExpressionValue(type, -value.get<bool>());
        default:
            Throw(InvalidTypeException, "Unable to handle type %s", type.getName());
    }
}


}
}
