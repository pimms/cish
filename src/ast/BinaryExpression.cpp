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
    // expressions should be casted to the type of the highest
    // TypeDecl::Type value.
    if (_left->getType().getType() >= _right->getType().getType()) {
        _workingType = _left->getType();
    } else {
        _workingType = _right->getType();
    }

	if (op >= __BOOLEAN_BOUNDARY) {
		_returnType = TypeDecl(TypeDecl::BOOL);

        // EDGECASE: When dealing with logical OR/AND, we need
        // to work with both terms of the expression as bool.
        // This will allow us to short-circuit properly.
        if (op == LOGICAL_OR || op == LOGICAL_AND) {
            _workingType = TypeDecl(TypeDecl::BOOL);
        }
    } else if (op >= __BITWISE_START && op <= __BITWISE_END) {
        // We will always promote the intrinsic type of the ecpression to int,
        // but we first need to make sure that the terms are not floaty.
        if (_workingType == TypeDecl::FLOAT || 
            _workingType == TypeDecl::DOUBLE || 
            _workingType == TypeDecl::POINTER)
        {
            Throw(InvalidOperationException, "Type '%s' cannot be used in bitwise operation",
                  _workingType.getName());
        }

        _returnType = TypeDecl(TypeDecl::INT);
        _workingType = TypeDecl(TypeDecl::INT);
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

    if (_left->getType().isFloating() || _right->getType().isFloating()) {
        floatSpecificChecks();
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
        case TypeDecl::LONG:    return evaluateT<long>(ctx);
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
    const uint32_t resultValue = evalPointerOperator(ptrVal, otherVal);

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
            case BITWISE_AND:
            case BITWISE_XOR:
            case BITWISE_OR:
            case BITWISE_LSHIFT:
            case BITWISE_RSHIFT:
                Throw(InvalidOperationException,
                    "Cannot perform arithmetics with two pointers");
            default:
                break;
        }
    } else {
        // Division and multiplication operations are never allowed on pointers
        if (_operator == MODULO || _operator == DIVIDE || _operator == MULTIPLY) {
            Throw(InvalidOperationException,
                "Invalid operation performed on pointer-type");
        }

        // The other type cannot be floating point
        TypeDecl::Type other = (left == TypeDecl::POINTER ? right : left);
        if (other == TypeDecl::FLOAT || other == TypeDecl::DOUBLE) {
            Throw(InvalidOperationException,
                "Cannot perform pointer arithmetics with floating point terms");
        }
    }
}

void BinaryExpression::floatSpecificChecks()
{
    if (_operator == MODULO) {
        Throw(InvalidOperationException, "Invalid operation performed on floating-point type");
    }
}

uint32_t BinaryExpression::evalPointerOperator(uint32_t a, uint32_t b) const
{
    switch (_operator) {
        case MULTIPLY:      return internal::multiply<uint32_t>(a, b);
        case DIVIDE:        return internal::safe_div<uint32_t>(a, b);
        case PLUS:          return internal::plus<uint32_t>(a, b);
        case MINUS:         return internal::minus<uint32_t>(a, b);
        case BITWISE_AND:   return internal::bitwiseAnd<int32_t>(a, b);
        case BITWISE_XOR:   return internal::bitwiseXor<int32_t>(a, b);
        case BITWISE_OR:    return internal::bitwiseOr<int32_t>(a, b);
        case BITWISE_LSHIFT:return internal::lshift<int32_t>(a, b);
        case BITWISE_RSHIFT:return internal::rshift<int32_t>(a, b);
        case GT:            return internal::greater<uint32_t>(a, b);
        case LT:            return internal::less<uint32_t>(a, b);
        case GTE:           return internal::greaterEqual<uint32_t>(a, b);
        case LTE:           return internal::lessEqual<uint32_t>(a, b);
        case EQ:            return internal::equalTo<uint32_t>(a, b);
        case NE:            return internal::notEqual<uint32_t>(a, b);
        case LOGICAL_AND:   return internal::logicalAnd<uint32_t>(a, b);
        case LOGICAL_OR:    return internal::logicalOr<uint32_t>(a, b);

        case MODULO: break; /* Explicitly handled below */
    }

    if (_operator == MODULO) {
        return internal::safe_mod<uint32_t>(a, b);
    }

    throw std::runtime_error("Operator unhandled: " + std::to_string(_operator));
}


}
}
