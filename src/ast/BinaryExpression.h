#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
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
        __BITWISE_END       = 92,

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

    template<typename T>
    ExpressionValue evaluateT(vm::ExecutionContext *ctx) const;

    ExpressionValue evaluatePtrT(vm::ExecutionContext *ctx) const;

    template<typename T>
    std::function<T(T,T)> getFunction() const;
};

template<typename T>
ExpressionValue BinaryExpression::evaluateT(vm::ExecutionContext *ctx) const
{
    ExpressionValue lval = _left->evaluate(ctx);
    ExpressionValue rval = _right->evaluate(ctx);

    // TODO: Check the RHS for 0, and throw div/0 VMRE

    if (_operator >= __BOOLEAN_BOUNDARY) {
        std::function<bool(T,T)> func = getFunction<T>();
        const bool value = func(lval.get<T>(), rval.get<T>());
        return ExpressionValue(TypeDecl(TypeDecl::Type::BOOL), value);
    } else {
        std::function<T(T,T)> func = getFunction<T>();
        const T value = func(lval.get<T>(), rval.get<T>());
        return ExpressionValue(TypeDecl::getFromNative<T>(), value);
    }
}

template<typename T>
std::function<T(T,T)> BinaryExpression::getFunction() const
{
    switch (_operator) {
        case MULTIPLY:      return std::multiplies<T>();
        case DIVIDE:        return std::divides<T>();
        case PLUS:          return std::plus<T>();
        case MINUS:         return std::minus<T>();
        case BITWISE_AND:   return std::bit_and<int32_t>();
        case BITWISE_XOR:   return std::bit_xor<int32_t>();
        case BITWISE_OR:    return std::bit_or<int32_t>();
        case GT:            return std::greater<T>();
        case LT:            return std::less<T>();
        case GTE:           return std::greater_equal<T>();
        case LTE:           return std::less_equal<T>();
        case EQ:            return std::equal_to<T>();
        case NE:            return std::not_equal_to<T>();
        case LOGICAL_AND:   return std::logical_and<T>();
        case LOGICAL_OR:    return std::logical_or<T>();

        case MODULO: break; /* Explicitly handled below */
    }

    if (_operator == MODULO) {
        if constexpr (std::is_floating_point<T>()) {
            // TODO: Make this a VM runtime error
            throw std::runtime_error("Modulo attempted on floating point number");
        } else {
            return std::modulus<T>();
        }
    }

    throw std::runtime_error("Operator unhandled: " + std::to_string(_operator));
}


}
}
