#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(DivisionByZeroException);

namespace internal
{

template<typename T>
T multiply(T a, T b)
{
    return a * b;
}

template<typename T>
T safe_div(T a, T b)
{
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a / b;
}

template<typename T>
T plus(T a, T b)
{
    return a + b;
}

template<typename T>
T minus(T a, T b)
{
    return a - b;
}

template<typename T>
T bitwiseAnd(T a, T b)
{
    return a & b;
}

template<typename T>
T bitwiseXor(T a, T b)
{
    return a ^ b;
}

template<typename T>
T bitwiseOr(T a, T b)
{
    return a | b;
}

template<typename T>
T lshift(T a, T n)
{
    return a << n;
}

template<typename T>
T rshift(T a, T n)
{
    return a >> n;
}

template<typename T>
T greater(T a, T b)
{
    return a > b;
}

template<typename T>
T less(T a, T b)
{
    return a < b;
}

template<typename T>
T greaterEqual(T a, T b)
{
    return a >= b;
}

template<typename T>
T lessEqual(T a, T b)
{
    return a <= b;
}

template<typename T>
T equalTo(T a, T b)
{
    return a == b;
}

template<typename T>
T notEqual(T a, T b)
{
    return a != b;
}

template<typename T>
T logicalAnd(T a, T b)
{
    return a && b;
}

template<typename T>
T logicalOr(T a, T b)
{
    return a || b;
}

template<typename T>
T safe_mod(T a, T b)
{
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a % b;
}

}


class BinaryExpression: public Expression
{
public:
    enum Operator
    {
        MULTIPLY            = 1,
        DIVIDE,
        MODULO,
        PLUS,
        MINUS,

        __BITWISE_START     = 90,
        BITWISE_AND         = 90,
        BITWISE_XOR         = 91,
        BITWISE_OR          = 92,
        BITWISE_LSHIFT      = 93,
        BITWISE_RSHIFT      = 94,
        __BITWISE_END       = 94,

        /* ALL OPERATORS BELOW THIS LINE MUST EVALUATE TO BOOL */
        __BOOLEAN_BOUNDARY  = 100,
        GT                  = 100,
        LT,
        GTE,
        LTE,
        EQ,
        NE,
        LOGICAL_AND,
        LOGICAL_OR
    };

    BinaryExpression(Operator op, Expression::Ptr left, Expression::Ptr right);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext *ctx) const override;

private:
    Operator _operator;
    TypeDecl _returnType;
    TypeDecl _workingType;
    Expression::Ptr _left;
    Expression::Ptr _right;

    void pointerSpecificChecks();
    void floatSpecificChecks();

    template<typename T>
    ExpressionValue evaluateT(vm::ExecutionContext *ctx) const;

    ExpressionValue evaluatePtrT(vm::ExecutionContext *ctx) const;

    template<typename T>
    T evalOperator(T a, T b) const;
};

template<typename T>
ExpressionValue BinaryExpression::evaluateT(vm::ExecutionContext *ctx) const
{
    ExpressionValue lval = _left->evaluate(ctx);
    ExpressionValue rval = _right->evaluate(ctx);

    if (_operator >= __BOOLEAN_BOUNDARY) {
        const bool value = evalOperator(lval.get<T>(), rval.get<T>());
        return ExpressionValue(TypeDecl(TypeDecl::Type::BOOL), value);
    } else {
        const T value = evalOperator(lval.get<T>(), rval.get<T>());
        return ExpressionValue(TypeDecl::getFromNative<T>(), value);
    }
}

template<typename T>
T BinaryExpression::evalOperator(T a, T b) const
{
    switch (_operator) {
        case MULTIPLY:      return internal::multiply<T>(a, b);
        case DIVIDE:        return internal::safe_div<T>(a, b);
        case PLUS:          return internal::plus<T>(a, b);
        case MINUS:         return internal::minus<T>(a, b);
        case BITWISE_AND:   return internal::bitwiseAnd<int32_t>(a, b);
        case BITWISE_XOR:   return internal::bitwiseXor<int32_t>(a, b);
        case BITWISE_OR:    return internal::bitwiseOr<int32_t>(a, b);
        case BITWISE_LSHIFT:return internal::lshift<int32_t>(a, b);
        case BITWISE_RSHIFT:return internal::rshift<int32_t>(a, b);
        case GT:            return internal::greater<T>(a, b);
        case LT:            return internal::less<T>(a, b);
        case GTE:           return internal::greaterEqual<T>(a, b);
        case LTE:           return internal::lessEqual<T>(a, b);
        case EQ:            return internal::equalTo<T>(a, b);
        case NE:            return internal::notEqual<T>(a, b);
        case LOGICAL_AND:   return internal::logicalAnd<T>(a, b);
        case LOGICAL_OR:    return internal::logicalOr<T>(a, b);

        case MODULO: break; /* Explicitly handled below */
    }

    if (_operator == MODULO) {
        if constexpr (std::is_floating_point<T>()) {
            // TODO: Make this a VM runtime error
            throw std::runtime_error("Modulo attempted on floating point number");
        } else {
            return internal::safe_mod<T>(a, b);
        }
    }

    throw std::runtime_error("Operator unhandled: " + std::to_string(_operator));
}


}
}
