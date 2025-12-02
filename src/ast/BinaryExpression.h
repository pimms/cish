#pragma once

#include "AstNodes.h"

namespace cish::ast
{
DECLARE_EXCEPTION(DivisionByZeroException);
}

namespace cish::ast::internal
{

template<typename T>
inline T multiply(const T& a, const T& b)
{
    return a * b;
}

template<typename T>
inline T safe_div(const T& a, const T& b)
{
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a / b;
}

template<typename T>
inline T plus(const T& a, const T& b)
{
    return a + b;
}

template<typename T>
inline T minus(const T& a, const T& b)
{
    return a - b;
}

template<typename T>
inline T bitwiseAnd(const T& a, const T& b)
{
    return a & b;
}

template<typename T>
inline T bitwiseXor(const T& a, const T& b)
{
    return a ^ b;
}

template<typename T>
inline T bitwiseOr(const T& a, const T& b)
{
    return a | b;
}

template<typename T>
inline T lshift(const T& a, const T& n)
{
    return a << n;
}

template<typename T>
inline T rshift(const T& a, const T& n)
{
    return a >> n;
}

template<typename T>
inline T greater(const T& a, const T& b)
{
    return a > b;
}

template<typename T>
inline T less(const T& a, const T& b)
{
    return a < b;
}

template<typename T>
inline T greaterEqual(const T& a, const T& b)
{
    return a >= b;
}

template<typename T>
inline T lessEqual(const T& a, const T& b)
{
    return a <= b;
}

template<typename T>
inline T equalTo(const T& a, const T& b)
{
    return a == b;
}

template<typename T>
inline T notEqual(const T& a, const T& b)
{
    return a != b;
}

template<typename T>
inline T logicalAnd(const T& a, const T& b)
{
    return a && b;
}

template<typename T>
inline T logicalOr(const T& a, const T& b)
{
    return a || b;
}

template<typename T>
inline T safe_mod(const T& a, const T& b)
{
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a % b;
}
}

namespace cish::ast
{

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

    uint32_t evalPointerOperator(uint32_t left, uint32_t right) const;

    template<typename T>
    ExpressionValue evaluateT(vm::ExecutionContext *ctx) const;

    ExpressionValue evaluatePtrT(vm::ExecutionContext *ctx) const;

    template<typename T>
    T evalWithContext(vm::ExecutionContext *ctx) const;

    template<typename T> T op_multiply(vm::ExecutionContext *ctx) const;
    template<typename T> T op_safe_div(vm::ExecutionContext *ctx) const;
    template<typename T> T op_plus(vm::ExecutionContext *ctx) const;
    template<typename T> T op_minus(vm::ExecutionContext *ctx) const;
    template<typename T> T op_bitwiseAnd(vm::ExecutionContext *ctx) const;
    template<typename T> T op_bitwiseXor(vm::ExecutionContext *ctx) const;
    template<typename T> T op_bitwiseOr(vm::ExecutionContext *ctx) const;
    template<typename T> T op_lshift(vm::ExecutionContext *ctx) const;
    template<typename T> T op_rshift(vm::ExecutionContext *ctx) const;
    template<typename T> T op_greater(vm::ExecutionContext *ctx) const;
    template<typename T> T op_less(vm::ExecutionContext *ctx) const;
    template<typename T> T op_greaterEqual(vm::ExecutionContext *ctx) const;
    template<typename T> T op_lessEqual(vm::ExecutionContext *ctx) const;
    template<typename T> T op_equalTo(vm::ExecutionContext *ctx) const;
    template<typename T> T op_notEqual(vm::ExecutionContext *ctx) const;
    template<typename T> T op_logicalAnd(vm::ExecutionContext *ctx) const;
    template<typename T> T op_logicalOr(vm::ExecutionContext *ctx) const;
    template<typename T> T op_safe_mod(vm::ExecutionContext *ctx) const;
};

template<typename T>
ExpressionValue BinaryExpression::evaluateT(vm::ExecutionContext *ctx) const
{
    if (_operator >= __BOOLEAN_BOUNDARY) {
        const bool value = evalWithContext<T>(ctx);
        return ExpressionValue(TypeDecl(TypeDecl::Type::BOOL), value);
    } else {
        const T value = evalWithContext<T>(ctx);
        return ExpressionValue(TypeDecl::getFromNative<T>(), value);
    }
}

template<typename T>
T BinaryExpression::evalWithContext(vm::ExecutionContext *ctx) const
{
    switch (_operator) {
        case MULTIPLY:      return op_multiply<T>(ctx);
        case DIVIDE:        return op_safe_div<T>(ctx);
        case PLUS:          return op_plus<T>(ctx);
        case MINUS:         return op_minus<T>(ctx);
        case BITWISE_AND:   return op_bitwiseAnd<int32_t>(ctx);
        case BITWISE_XOR:   return op_bitwiseXor<int32_t>(ctx);
        case BITWISE_OR:    return op_bitwiseOr<int32_t>(ctx);
        case BITWISE_LSHIFT:return op_lshift<int32_t>(ctx);
        case BITWISE_RSHIFT:return op_rshift<int32_t>(ctx);
        case GT:            return op_greater<T>(ctx);
        case LT:            return op_less<T>(ctx);
        case GTE:           return op_greaterEqual<T>(ctx);
        case LTE:           return op_lessEqual<T>(ctx);
        case EQ:            return op_equalTo<T>(ctx);
        case NE:            return op_notEqual<T>(ctx);
        case LOGICAL_AND:   return op_logicalAnd<T>(ctx);
        case LOGICAL_OR:    return op_logicalOr<T>(ctx);

        case MODULO: break; /* Explicitly handled below */
    }

    if (_operator == MODULO) {
        if constexpr (std::is_floating_point<T>()) {
            // TODO: Make this a VM runtime error
            throw std::runtime_error("Modulo attempted on floating point number");
        } else {
            return op_safe_mod<T>(ctx);
        }
    }

    throw std::runtime_error("Operator unhandled: " + std::to_string(_operator));
}


template<typename T>
inline T BinaryExpression::op_multiply(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a * b;
}

template<typename T>
inline T BinaryExpression::op_safe_div(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a / b;
}

template<typename T>
inline T BinaryExpression::op_plus(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a + b;
}

template<typename T>
inline T BinaryExpression::op_minus(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a - b;
}

template<typename T>
inline T BinaryExpression::op_bitwiseAnd(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a & b;
}

template<typename T>
inline T BinaryExpression::op_bitwiseXor(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a ^ b;
}

template<typename T>
inline T BinaryExpression::op_bitwiseOr(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a | b;
}

template<typename T>
inline T BinaryExpression::op_lshift(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a << b;
}

template<typename T>
inline T BinaryExpression::op_rshift(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a >> b;
}

template<typename T>
inline T BinaryExpression::op_greater(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a > b;
}

template<typename T>
inline T BinaryExpression::op_less(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a < b;
}

template<typename T>
inline T BinaryExpression::op_greaterEqual(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a >= b;
}

template<typename T>
inline T BinaryExpression::op_lessEqual(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a <= b;
}

template<typename T>
inline T BinaryExpression::op_equalTo(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a == b;
}

template<typename T>
inline T BinaryExpression::op_notEqual(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    return a != b;
}

template<typename T>
inline T BinaryExpression::op_logicalAnd(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    if (a) {
        T b = _right->evaluate(ctx).get<T>();
        return b;
    }

    return T(0);
}

template<typename T>
inline T BinaryExpression::op_logicalOr(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    if (a) {
        return T(1);
    }

    T b = _right->evaluate(ctx).get<T>();
    return b;
}

template<typename T>
inline T BinaryExpression::op_safe_mod(vm::ExecutionContext *ctx) const
{
    T a = _left->evaluate(ctx).get<T>();
    T b = _right->evaluate(ctx).get<T>();
    if (b == 0) {
        Throw(DivisionByZeroException, "Division by zero");
    }
    return a % b;
}

}
