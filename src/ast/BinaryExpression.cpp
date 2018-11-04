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

    // Make sure that both sub-expressions are castable to both working and return type
    if (!_left->getType().castableTo(_workingType)) {
        Throw(InvalidCastException, "Type '%s' is not convertible to '%s'",
                _left->getType().getName(), _workingType.getName());
    }

    if (!_left->getType().castableTo(_returnType)) {
        Throw(InvalidCastException, "Type '%s' is not convertible to '%s'",
                _left->getType().getName(), _returnType.getName());
    }

    if (!_right->getType().castableTo(_workingType)) {
        Throw(InvalidCastException, "Type '%s' is not convertible to '%s'",
                _right->getType().getName(), _workingType.getName());
    }

    if (!_right->getType().castableTo(_returnType)) {
        Throw(InvalidCastException, "Type '%s' is not convertible to '%s'",
                _right->getType().getName(), _returnType.getName());
    }
}

BinaryExpression::~BinaryExpression()
{
    delete _left;
    delete _right;
}

TypeDecl BinaryExpression::getType() const
{
    return _returnType;
}

ExpressionValue BinaryExpression::evaluate(vm::ExecutionContext *ctx)
{
    // Start by folding the types
    switch (_workingType.getType()) {
        case TypeDecl::BOOL:    return evaluateT<bool>(ctx);
        case TypeDecl::CHAR:    return evaluateT<char>(ctx);
        case TypeDecl::SHORT:   return evaluateT<short>(ctx);
        case TypeDecl::INT:     return evaluateT<int>(ctx);
        case TypeDecl::LONG:    return evaluateT<long>(ctx);
        case TypeDecl::FLOAT:   return evaluateT<float>(ctx);
        case TypeDecl::DOUBLE:  return evaluateT<double>(ctx);

        default:
            Throw(ExpressionTypeException, "Unable to handle type in binary expression '%d'", _workingType.getType());
    }
}


}
}
