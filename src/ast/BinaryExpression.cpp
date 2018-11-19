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
BinaryExpression::BinaryExpression(Operator op, Expression::Ptr left, Expression::Ptr right):
    _operator(op),
    _left(left),
    _right(right)
{
    // The max works because of the order in the TypeDecl::Type-enum. If two
    // expressions of different types are evaluated in conjunction, both
    // expressions should be casted to // the type of the highest
    // TypeDecl::Type value.
    if (_left->getType().getType() >= _right->getType().getType()) {
        _workingType = _left->getType();
    } else {
        _workingType = _right->getType();
    }

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

    if (_left->getType().getType() == TypeDecl::POINTER ||
        _right->getType().getType() == TypeDecl::POINTER) {
        pointerSpecificChecks();
    }
}

TypeDecl BinaryExpression::getType() const
{
    return _returnType;
}

ExpressionValue BinaryExpression::evaluate(vm::ExecutionContext *ctx) const
{
    // Start by folding the types
    switch (_workingType.getType()) {
        case TypeDecl::BOOL:    return evaluateT<bool>(ctx);
        case TypeDecl::CHAR:    return evaluateT<char>(ctx);
        case TypeDecl::SHORT:   return evaluateT<short>(ctx);
        case TypeDecl::INT:     return evaluateT<int>(ctx);
        case TypeDecl::FLOAT:   return evaluateT<float>(ctx);
        case TypeDecl::DOUBLE:  return evaluateT<double>(ctx);
        case TypeDecl::POINTER: return evaluatePtrT(ctx);

        default:
            Throw(ExpressionTypeException,
                "Unable to handle type in binary expression '%d'",
                _workingType.getType());
    }
}

ExpressionValue BinaryExpression::evaluatePtrT(vm::ExecutionContext *ctx) const
{
    // We're dealing with a few different cases here:
    //  - Comparison between pointers
    //  - Arithmetics performed on pointers
    // In either case, we know that at least one of the expressions is of type
    // pointer, while the other may be integral or a pointer.
    Expression *ptr, *maybePtr;
    if (_left->getType().getType() == TypeDecl::POINTER) {
        ptr = _left.get();
        maybePtr = _right.get();
    } else {
        ptr = _right.get();
        maybePtr = _left.get();
    }

    // Evaluate both terms
    const uint32_t ptrVal = ptr->evaluate(ctx).get<uint32_t>();
    uint32_t otherVal = maybePtr->evaluate(ctx).get<uint32_t>();

    // If the other type is not of type pointer, and the operation is arithmetic
    // in nature, we need to multiply the non-pointer operand.
    if (_operator < Operator::__BOOLEAN_BOUNDARY && maybePtr->getType().getType() != TypeDecl::POINTER) {
        uint32_t sizeContext = 1;
        if (ptr->getType().getReferencedType()->getType() == TypeDecl::VOID) {
            sizeContext = 1;
        } else {
            sizeContext = ptr->getType().getReferencedType()->getSize();
        }
        otherVal *= sizeContext;
    }

    // Perform the operation, generate a result with the same type
    // as the pointer-type.
    auto func = getFunction<uint32_t>();
    const uint32_t resultValue = func(ptrVal, otherVal);

    return ExpressionValue(ptr->getType(), resultValue);
}

void BinaryExpression::pointerSpecificChecks()
{
    TypeDecl::Type left = _left->getType().getType();
    TypeDecl::Type right = _right->getType().getType();

    // If both left and right are pointers, we are only allowed
    // to perform comparisons.
    if (left == TypeDecl::POINTER && right == TypeDecl::POINTER) {
        switch (_operator) {
            case MULTIPLY:
            case DIVIDE:
            case MODULO:
            case PLUS:
            case MINUS:
                Throw(InvalidOperationException,
                    "Cannot perform arithmetics with two pointers");
            default:
                break;
        }
    } else {
        // Division and multiplication operations are never allowed on pointers
        if (_operator == MODULO || _operator == DIVIDE || _operator == MULTIPLY) {
            Throw(InvalidOperationException,
                "Invalid operation perform on pointer-type");
        }

        // The other type cannot be floating point
        TypeDecl::Type other = (left == TypeDecl::POINTER ? right : left);
        if (other == TypeDecl::FLOAT || other == TypeDecl::DOUBLE) {
            Throw(InvalidOperationException,
                "Cannot perform pointer arithmetics with floating point terms");
        }
    }
}


}
}
