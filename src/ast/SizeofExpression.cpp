#include "SizeofExpression.h"

namespace cish
{
namespace ast
{


SizeofExpression::SizeofExpression(Expression::Ptr expression)
{
    _size = expression->getType().getSize();
}

SizeofExpression::SizeofExpression(TypeDecl type)
{
    _size = type.getSize();
}

TypeDecl SizeofExpression::getType() const
{
    return TypeDecl::INT;
}

ExpressionValue SizeofExpression::evaluate(vm::ExecutionContext *context) const
{
    return ExpressionValue(TypeDecl::INT, _size);
}


}
}

