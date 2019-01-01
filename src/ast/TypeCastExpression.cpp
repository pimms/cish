#include "TypeCastExpression.h"


namespace cish
{
namespace ast
{


TypeCastExpression::TypeCastExpression(TypeDecl type, Expression::Ptr expr):
    _type(type),
    _expression(expr)
{
    if (!_expression->getType().castableTo(_type)) {
        Throw(InvalidCastException, "Cannot cast expression of type %s to %s",
                _expression->getType().getName(), _type.getName());
    }
}

TypeDecl TypeCastExpression::getType() const
{
    return _type;
}

ExpressionValue TypeCastExpression::evaluate(vm::ExecutionContext *ctx) const
{
    ExpressionValue uncasted = _expression->evaluate(ctx);
    ExpressionValue casted = 0;

    switch (_type.getType()) {
        case TypeDecl::VOID:
            casted = ExpressionValue(TypeDecl::VOID);
            break;
        case TypeDecl::BOOL:
            casted = ExpressionValue(_type, uncasted.get<bool>());
            break;
        case TypeDecl::CHAR:
            casted = ExpressionValue(_type, uncasted.get<int8_t>());
            break;
        case TypeDecl::SHORT:
            casted = ExpressionValue(_type, uncasted.get<int16_t>());
            break;
        case TypeDecl::INT:
            casted = ExpressionValue(_type, uncasted.get<int32_t>());
            break;
        case TypeDecl::FLOAT:
            casted = ExpressionValue(_type, uncasted.get<float>());
            break;
        case TypeDecl::DOUBLE:
            casted = ExpressionValue(_type, uncasted.get<double>());
            break;
        case TypeDecl::POINTER:
            casted = ExpressionValue(_type, uncasted.get<uint32_t>());
            break;
        case TypeDecl::STRUCT:
            casted = ExpressionValue(_type, uncasted.get<uint32_t>());
            break;
    }

    return casted;
}



}
}
