#include "BinaryExpression.h"

namespace cish
{
namespace ast
{


/*
==============
BinaryExpression
==============
*/
BinaryExpression::BinaryExpression(Operator op, Expression *left, Expression *right):
    _operator(op),
    _left(left),
    _right(right)
{
    // The max works because of the order in the TypeDecl::Type-enum. If two expressions of
    // different types are evaluated in conjunction, both expressions should be casted to
    // the type of the highest TypeDecl::Type value.
    _workingType = std::max(_left->getType(). getType(), _right->getType().getType());

	if (op >= __BOOLEAN_BOUNDARY) {
		_returnType = TypeDecl(TypeDecl::BOOL);
	} else {
        _returnType = _workingType;
	}
}

TypeDecl BinaryExpression::getType() const
{
    return _returnType;
}

ExpressionValue BinaryExpression::evaluate()
{
    // Start by folding the types
    switch (_workingType.getType()) {
        case TypeDecl::BOOL:    return evaluateT<bool>();
        case TypeDecl::CHAR:    return evaluateT<char>();
        case TypeDecl::SHORT:   return evaluateT<short>();
        case TypeDecl::INT:     return evaluateT<int>();
        case TypeDecl::LONG:    return evaluateT<long>();
        case TypeDecl::FLOAT:   return evaluateT<float>();
        case TypeDecl::DOUBLE:  return evaluateT<double>();

        default:
            Throw(ExpressionTypeException, "Unable to handle type in binary expression '%d'", _workingType.getType());
    }
}


}
}
