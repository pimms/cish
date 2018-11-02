#include <gtest/gtest.h>

#include "ast/BinaryExpression.h"
#include "vm/ExecutionContext.h"

using namespace cish::ast;


template<typename T>
static LiteralExpression* expr(T value)
{
    TypeDecl type = TypeDecl::getFromNative<T>();
    ExpressionValue exprValue(type, value);
    return new LiteralExpression(exprValue);
}

template<typename LHST, typename RHST, typename ResT>
static void testTypeFolding()
{
    auto left = expr<LHST>(0);
    auto right = expr<RHST>(0);

    // Addition has no impact on type, and works for all types
    BinaryExpression expr(BinaryExpression::PLUS, left, right);

    ASSERT_EQ(TypeDecl::getFromNative<ResT>(), expr.getType());
}


TEST(BinaryExpressionTest, type_folding)
{
    // Could probably be shortened quite a bit, but this way
    // we at least test every single conversion case, lol

    // Base bool
    testTypeFolding<bool,bool,bool>();

    testTypeFolding<short,bool,short>();
    testTypeFolding<short,int,int>();

    testTypeFolding<int,bool,int>();
    testTypeFolding<bool,int,int>();

    testTypeFolding<long,bool,long>();
    testTypeFolding<bool,long,long>();

    testTypeFolding<float,bool,float>();
    testTypeFolding<bool,float,float>();

    testTypeFolding<double,bool,double>();
    testTypeFolding<bool,double,double>();

    // Base char
    testTypeFolding<char,char,char>();

    testTypeFolding<short,char,short>();
    testTypeFolding<short,int,int>();

    testTypeFolding<int,char,int>();
    testTypeFolding<char,int,int>();

    testTypeFolding<long,char,long>();
    testTypeFolding<char,long,long>();

    testTypeFolding<float,char,float>();
    testTypeFolding<char,float,float>();

    testTypeFolding<double,char,double>();
    testTypeFolding<char,double,double>();

    // Base short
    testTypeFolding<short,short,short>();

    testTypeFolding<int,short,int>();
    testTypeFolding<short,int,int>();

    testTypeFolding<long,short,long>();
    testTypeFolding<short,long,long>();

    testTypeFolding<float,short,float>();
    testTypeFolding<short,float,float>();

    testTypeFolding<double,short,double>();
    testTypeFolding<short,double,double>();

    // Base int
    testTypeFolding<int,int,int>();

    testTypeFolding<long,int,long>();
    testTypeFolding<int,long,long>();

    testTypeFolding<float,int,float>();
    testTypeFolding<int,float,float>();

    testTypeFolding<double,int,double>();
    testTypeFolding<int,double,double>();

    // Base long
    testTypeFolding<long,long,long>();

    testTypeFolding<float,long,float>();
    testTypeFolding<long,float,float>();

    testTypeFolding<double,long,double>();
    testTypeFolding<long,double,double>();

    // Base float
    testTypeFolding<float,float,float>();

    testTypeFolding<double,float,double>();
    testTypeFolding<float,double,double>();

    // Base double
    testTypeFolding<double,double,double>();
}




template<typename LHST, typename RHST, typename ResT>
static void testBinaryExpr(BinaryExpression::Operator op, LHST lval, RHST rval, ResT expected)
{
    cish::vm::Memory memory(100, 1);
    cish::vm::ExecutionContext econtext(&memory);
    auto left = expr<LHST>(lval);
    auto right = expr<RHST>(rval);

    BinaryExpression expr(op, left, right);
    ExpressionValue result = expr.evaluate(&econtext);

    ASSERT_EQ(TypeDecl::getFromNative<ResT>(), result.getIntrinsicType());

    if constexpr (std::is_floating_point<ResT>()) {
        ASSERT_NEAR(expected, result.get<ResT>(), 0.001);
    } else {
        ASSERT_EQ(expected, result.get<ResT>());
    }
}


template<typename LHST, typename RHST, typename ResT>
static void testArithmeticOperators()
{
    typedef std::function<ResT(ResT,ResT)> Func;
    std::map<BinaryExpression::Operator,Func> operators;

    if constexpr (std::is_floating_point<ResT>()) {
        operators = {
            {BinaryExpression::MULTIPLY, std::multiplies<ResT>() },
            {BinaryExpression::DIVIDE, std::divides<ResT>() },
          /*{BinaryExpression::MODULO, std::modulus<ResT>() },*/
            {BinaryExpression::PLUS, std::plus<ResT>() },
            {BinaryExpression::MINUS, std::minus<ResT>() },
        };
    } else {
        operators = {
            {BinaryExpression::MULTIPLY, std::multiplies<ResT>() },
            {BinaryExpression::DIVIDE, std::divides<ResT>() },
            {BinaryExpression::MODULO, std::modulus<ResT>() },
            {BinaryExpression::PLUS, std::plus<ResT>() },
            {BinaryExpression::MINUS, std::minus<ResT>() },
        };
    }

    const std::vector<std::pair<LHST,RHST>> testData = {
        {100.5, 150},
        {100.5, 50},
        {10.5, 50},
        {2.5, 1.5},
        {1.5, 1.5},
        {0.5, 1.5},
        {0, 1.5},
        {-100, -150.5},
        {-100, -50.5},
        {-10.5, -50.5},
        {-2.8, -1.3},
        {-1.5, -1},
        {-0, -1.5},
        {-0.5, -1},
    };

    for (auto op: operators) {
        for (auto pair: testData) {
            ResT expected = op.second((LHST)pair.first, (RHST)pair.second);
            testBinaryExpr<LHST,RHST,ResT>(op.first, (LHST)pair.first, (RHST)pair.second, expected);
        }
    }
}

TEST(BinaryExpressionTest, testAllArithmeticOperatorPermutations)
{
    testArithmeticOperators<bool,bool,bool>();

    testArithmeticOperators<char,bool,char>();
    testArithmeticOperators<char,int,int>();

    testArithmeticOperators<short,bool,short>();
    testArithmeticOperators<short,int,int>();

    testArithmeticOperators<int,bool,int>();
    testArithmeticOperators<bool,int,int>();

    testArithmeticOperators<long,bool,long>();
    testArithmeticOperators<bool,long,long>();

    testArithmeticOperators<float,bool,float>();
    testArithmeticOperators<bool,float,float>();

    testArithmeticOperators<double,bool,double>();
    testArithmeticOperators<bool,double,double>();

    // Base char
    testArithmeticOperators<char,char,char>();

    testArithmeticOperators<short,char,short>();
    testArithmeticOperators<short,int,int>();

    testArithmeticOperators<int,char,int>();
    testArithmeticOperators<char,int,int>();

    testArithmeticOperators<long,char,long>();
    testArithmeticOperators<char,long,long>();

    testArithmeticOperators<float,char,float>();
    testArithmeticOperators<char,float,float>();

    testArithmeticOperators<double,char,double>();
    testArithmeticOperators<char,double,double>();

    // Base short
    testArithmeticOperators<short,short,short>();

    testArithmeticOperators<int,short,int>();
    testArithmeticOperators<short,int,int>();

    testArithmeticOperators<long,short,long>();
    testArithmeticOperators<short,long,long>();

    testArithmeticOperators<float,short,float>();
    testArithmeticOperators<short,float,float>();

    testArithmeticOperators<double,short,double>();
    testArithmeticOperators<short,double,double>();

    // Base int
    testArithmeticOperators<int,int,int>();

    testArithmeticOperators<long,int,long>();
    testArithmeticOperators<int,long,long>();

    testArithmeticOperators<float,int,float>();
    testArithmeticOperators<int,float,float>();

    testArithmeticOperators<double,int,double>();
    testArithmeticOperators<int,double,double>();

    // Base long
    testArithmeticOperators<long,long,long>();

    testArithmeticOperators<float,long,float>();
    testArithmeticOperators<long,float,float>();

    testArithmeticOperators<double,long,double>();
    testArithmeticOperators<long,double,double>();

    // Base float
    testArithmeticOperators<float,float,float>();

    testArithmeticOperators<double,float,double>();
    testArithmeticOperators<float,double,double>();

    // Base double
    testArithmeticOperators<double,double,double>();
}


template<typename T>
static void testLogicalOperators()
{
    typedef std::function<bool(T,T)> Func;
    const std::map<BinaryExpression::Operator,Func> operators = {
        {BinaryExpression::GT, std::greater<T>() },
        {BinaryExpression::LT, std::less<T>() },
        {BinaryExpression::GTE, std::greater_equal<T>() },
        {BinaryExpression::LTE, std::less_equal<T>() },
        {BinaryExpression::EQ, std::equal_to<T>() },
        {BinaryExpression::NE, std::not_equal_to<T>() },
        {BinaryExpression::LOGICAL_AND, std::logical_and<T>() },
        {BinaryExpression::LOGICAL_OR, std::logical_or<T>() },
    };

    const std::vector<std::pair<T,T>> testData = {
        {0, 0},
        {0, 1},
        {0, 1.5},
        {0, 0.5},
        {1, 1},
        {1, 0.01},
        {0.5, -0.5},
        {0.4, 0.51},
        {1, 1.9},
    };

    for (auto op: operators) {
        for (auto data: testData) {
            const bool expected = op.second((T)data.first, (T)data.second);
            testBinaryExpr<T,T,bool>(op.first, (T)data.first, (T)data.second, expected);
        }
    }
}

TEST(BinaryExpressionTest, testLogicalOperators)
{
    testLogicalOperators<bool>();
    testLogicalOperators<char>();
    testLogicalOperators<short>();
    testLogicalOperators<int>();
    testLogicalOperators<long>();
    testLogicalOperators<float>();
    testLogicalOperators<double>();
}


TEST(BinaryExpressionTest, simpleTestOfNestedBinaryExpressions)
{
    // int(100) * float(2) = float(200)
    auto i100 = expr<int>(100);
    auto f2 = expr<float>(2);
    BinaryExpression *left = new BinaryExpression(BinaryExpression::MULTIPLY, i100, f2);

    // char(-5) + int(295) = int(290)
    auto cm5 = expr<char>(-5);
    auto i295 = expr<int>(295);
    BinaryExpression *right = new BinaryExpression(BinaryExpression::PLUS, cm5, i295);

    // float(200) - int(290) = float(-90)
    BinaryExpression expr(BinaryExpression::MINUS, left, right);
    ASSERT_EQ(TypeDecl::FLOAT, expr.getType().getType());

    cish::vm::Memory memory(100, 1);
    cish::vm::ExecutionContext econtext(&memory);
    ASSERT_NEAR(-90.f, expr.evaluate(&econtext).get<float>(), 0.001);
}

