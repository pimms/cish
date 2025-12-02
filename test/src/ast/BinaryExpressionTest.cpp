#include <exception>
#include <gtest/gtest.h>

#include "ast/BinaryExpression.h"
#include "ast/LiteralExpression.h"
#include "vm/ExecutionContext.h"

#include <memory>

using namespace cish::ast;


template<typename T>
static LiteralExpression::Ptr expr(T value)
{
    TypeDecl type = TypeDecl::getFromNative<T>();
    ExpressionValue exprValue(type, value);
    return std::make_shared<LiteralExpression>(exprValue);
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

    testTypeFolding<float,char,float>();
    testTypeFolding<char,float,float>();

    testTypeFolding<double,char,double>();
    testTypeFolding<char,double,double>();

    // Base short
    testTypeFolding<short,short,short>();

    testTypeFolding<int,short,int>();
    testTypeFolding<short,int,int>();

    testTypeFolding<float,short,float>();
    testTypeFolding<short,float,float>();

    testTypeFolding<double,short,double>();
    testTypeFolding<short,double,double>();

    // Base int
    testTypeFolding<int,int,int>();

    testTypeFolding<float,int,float>();
    testTypeFolding<int,float,float>();

    testTypeFolding<double,int,double>();
    testTypeFolding<int,double,double>();


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

    for (const auto& op: operators) {
        for (const auto& pair: testData) {
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

    testArithmeticOperators<bool, long, long>();
    testArithmeticOperators<long, bool, long>();

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

    testArithmeticOperators<char, long, long>();
    testArithmeticOperators<long, char, long>();

    testArithmeticOperators<float,char,float>();
    testArithmeticOperators<char,float,float>();

    testArithmeticOperators<double,char,double>();
    testArithmeticOperators<char,double,double>();

    // Base short
    testArithmeticOperators<short,short,short>();

    testArithmeticOperators<int,short,int>();
    testArithmeticOperators<short,int,int>();

    testArithmeticOperators<short, long, long>();
    testArithmeticOperators<long, short, long>();

    testArithmeticOperators<float,short,float>();
    testArithmeticOperators<short,float,float>();

    testArithmeticOperators<double,short,double>();
    testArithmeticOperators<short,double,double>();

    // Base int
    testArithmeticOperators<int,int,int>();

    testArithmeticOperators<float,int,float>();
    testArithmeticOperators<int,float,float>();

    testArithmeticOperators<long,int,long>();
    testArithmeticOperators<int,long,long>();

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
    testLogicalOperators<float>();
    testLogicalOperators<double>();
}


template<typename LHST, typename RHST>
static void testBitwiseOperators()
{
    typedef std::function<int32_t(LHST,RHST)> Func;

    std::map<BinaryExpression::Operator,Func> operators = {
        {BinaryExpression::BITWISE_AND, std::bit_and<int32_t>() },
        {BinaryExpression::BITWISE_XOR, std::bit_xor<int32_t>() },
        {BinaryExpression::BITWISE_OR, std::bit_or<int32_t>() },
        {BinaryExpression::BITWISE_LSHIFT, [](int32_t a, int32_t b) -> int32_t { return a << b; } },
        {BinaryExpression::BITWISE_RSHIFT, [](int32_t a, int32_t b) -> int32_t { return a >> b; } },
    };

    const std::vector<std::pair<LHST,RHST>> testData = {
        {0x0101, 0x0001},
        {0x0101, 0x0001},
        {0xf003, 0x0000},
        {0x0, 0xFFFFFFFF},
        {0x7, 0xFFFFFFFF},
    };

    for (auto op: operators) {
        printf("operator: %d\n", (int)op.first);
        for (auto pair: testData) {
            int32_t expected = op.second((LHST)pair.first, (RHST)pair.second);
            testBinaryExpr<LHST,RHST,int32_t>(op.first, (LHST)pair.first, (RHST)pair.second, expected);
        }
    }
}

TEST(BinaryExpressionTest, testBitwiseOperators)
{
    testBitwiseOperators<bool, bool>();
    testBitwiseOperators<bool, char>();
    testBitwiseOperators<bool, short>();
    testBitwiseOperators<bool, int>();

    testBitwiseOperators<char, bool>();
    testBitwiseOperators<char, char>();
    testBitwiseOperators<char, short>();
    testBitwiseOperators<char, int>();

    testBitwiseOperators<short, bool>();
    testBitwiseOperators<short, char>();
    testBitwiseOperators<short, short>();
    testBitwiseOperators<short, int>();

    testBitwiseOperators<int, bool>();
    testBitwiseOperators<int, char>();
    testBitwiseOperators<int, short>();
    testBitwiseOperators<int, int>();
}


TEST(BinaryExpressionTest, simpleTestOfNestedBinaryExpressions)
{
    // int(100) * float(2) = float(200)
    auto i100 = expr<int>(100);
    auto f2 = expr<float>(2);
    BinaryExpression::Ptr left = std::make_shared<BinaryExpression>(BinaryExpression::MULTIPLY, i100, f2);

    // char(-5) + int(295) = int(290)
    auto cm5 = expr<char>(-5);
    auto i295 = expr<int>(295);
    BinaryExpression::Ptr right = std::make_shared<BinaryExpression>(BinaryExpression::PLUS, cm5, i295);

    // float(200) - int(290) = float(-90)
    BinaryExpression expr(BinaryExpression::MINUS, left, right);
    ASSERT_EQ(TypeDecl::FLOAT, expr.getType().getType());

    cish::vm::Memory memory(100, 1);
    cish::vm::ExecutionContext econtext(&memory);
    ASSERT_NEAR(-90.f, expr.evaluate(&econtext).get<float>(), 0.001);
}

TEST(BinaryExpressionTest, pointerOnPointerActionIsSinful)
{
    auto left = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
    auto right = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
    ASSERT_THROW(BinaryExpression expr(BinaryExpression::PLUS, left, right), InvalidOperationException);
}

TEST(BinaryExpressionTest, pointerArithmeticTypeTest)
{
    std::map<TypeDecl::Type, bool> typeMap = {
        {TypeDecl::Type::VOID,      false},
        {TypeDecl::Type::BOOL,      true},
        {TypeDecl::Type::CHAR,      true},
        {TypeDecl::Type::SHORT,     true},
        {TypeDecl::Type::INT,       true},
        {TypeDecl::Type::FLOAT,     false},
        {TypeDecl::Type::DOUBLE,    false},
    };

    for (auto pair: typeMap) {
        if (pair.second) {
            auto ptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            auto other = std::make_shared<LiteralExpression>(ExpressionValue(pair.first, 1));
            ASSERT_NO_THROW(BinaryExpression expr(BinaryExpression::PLUS, ptr, other));

            ptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            other = std::make_shared<LiteralExpression>(ExpressionValue(pair.first, 1));
            ASSERT_NO_THROW(BinaryExpression expr(BinaryExpression::PLUS, other, ptr));
        } else {
            auto ptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            auto other = std::make_shared<LiteralExpression>(ExpressionValue(pair.first, 1));
            ASSERT_ANY_THROW(BinaryExpression expr(BinaryExpression::PLUS, ptr, other));
            ASSERT_ANY_THROW(BinaryExpression expr(BinaryExpression::PLUS, other, ptr));
        }
    }
}

TEST(BinaryExpressionTest, verifyValidPointerOperations)
{
    std::map<BinaryExpression::Operator, bool> expected = {
        {BinaryExpression::MULTIPLY,      false},
        {BinaryExpression::DIVIDE,        false},
        {BinaryExpression::MODULO,        false},
        {BinaryExpression::PLUS,          true},
        {BinaryExpression::MINUS,         true},
        {BinaryExpression::GT,            true},
        {BinaryExpression::LT,            true},
        {BinaryExpression::GTE,           true},
        {BinaryExpression::LTE,           true},
        {BinaryExpression::EQ,            true},
        {BinaryExpression::NE,            true},
        {BinaryExpression::LOGICAL_AND,   true},
        {BinaryExpression::LOGICAL_OR,    true},
    };

    for (auto pair: expected) {
        if (pair.second) {
            auto eint = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 1));
            auto eptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            ASSERT_NO_THROW(BinaryExpression expr(pair.first, eptr, eint));

            eint = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 1));
            eptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            ASSERT_NO_THROW(BinaryExpression expr(pair.first, eint, eptr));
        } else {
            auto eint = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 1));
            auto eptr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
            ASSERT_THROW(BinaryExpression expr(pair.first, eptr, eint), InvalidOperationException);
            ASSERT_THROW(BinaryExpression expr(pair.first, eint, eptr), InvalidOperationException);
        }
    }
}

TEST(BinaryExpressionTest, pointerAdditionArithmetics)
{
    // We'll perform arithmetics on pointers of the following types:
    std::vector<TypeDecl> ptrTypes = {
        TypeDecl::VOID,
        TypeDecl::BOOL,
        TypeDecl::CHAR,
        TypeDecl::SHORT,
        TypeDecl::INT,
        TypeDecl::FLOAT,
        TypeDecl::DOUBLE,
        TypeDecl::getPointer(TypeDecl::VOID),
        TypeDecl::getPointer(TypeDecl::BOOL),
        TypeDecl::getPointer(TypeDecl::CHAR),
        TypeDecl::getPointer(TypeDecl::SHORT),
        TypeDecl::getPointer(TypeDecl::INT),
        TypeDecl::getPointer(TypeDecl::FLOAT),
        TypeDecl::getPointer(TypeDecl::DOUBLE),
    };

    for (auto ptrType: ptrTypes) {
        const int tsize = (ptrType == TypeDecl::VOID ? 1 : TypeDecl(ptrType).getSize());

        for (int i=-5; i<=5; i++) {
            const int initial = 100;
            const int expected = 100 + (i * tsize);

            cish::vm::Memory memory(100, 1);
            cish::vm::ExecutionContext econtext(&memory);

            TypeDecl ptrTermType = TypeDecl::getPointer(ptrType);

            auto ptrTerm = std::make_shared<LiteralExpression>(ExpressionValue(ptrTermType, initial));
            auto addTerm = std::make_shared<LiteralExpression>(ExpressionValue(i));
            BinaryExpression expr(BinaryExpression::PLUS, addTerm, ptrTerm);
            auto result = expr.evaluate(&econtext);
            ASSERT_EQ(expected, result.get<int>());
            ASSERT_TRUE(result.getIntrinsicType() == ptrTermType);
        }
    }
}

TEST(BinaryExpressionTest, invalidOperationsOnPointers)
{
    auto pointer = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 100));
    auto integer = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 100));

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_OR, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_OR, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_XOR, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_XOR, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_AND, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_AND, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_LSHIFT, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_LSHIFT, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_RSHIFT, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_RSHIFT, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MULTIPLY, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MULTIPLY, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::DIVIDE, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::DIVIDE, integer, pointer), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MODULO, pointer, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MODULO, integer, pointer), InvalidOperationException);
}

TEST(BinaryExpressionTest, invalidFloatOperations)
{
    auto floating = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::FLOAT, 10.f));
    auto integer = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::FLOAT, 10.f));

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_OR, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_OR, integer, floating), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_XOR, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_XOR, integer, floating), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_AND, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_AND, integer, floating), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_LSHIFT, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_LSHIFT, integer, floating), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_RSHIFT, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::BITWISE_RSHIFT, integer, floating), InvalidOperationException);

    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MODULO, floating, integer), InvalidOperationException);
    EXPECT_THROW(BinaryExpression expr(BinaryExpression::MODULO, integer, floating), InvalidOperationException);
}


class TestExpression: public Expression
{
public:
    TestExpression() { wasEvaluated = false; }

    TypeDecl getType() const override { return TypeDecl::INT; }

    ExpressionValue evaluate(cish::vm::ExecutionContext *) const override
    {
        wasEvaluated = true;
        return ExpressionValue(TypeDecl::INT, 0);
    }

    mutable bool wasEvaluated;
};

TEST(BinaryExpressionTest, logicalAndShortCircuit)
{
    cish::vm::Memory memory(100, 1);
    cish::vm::ExecutionContext econtext(&memory);

    auto falseExpr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 0));
    auto secondExpr = std::make_shared<TestExpression>();

    BinaryExpression exp(BinaryExpression::LOGICAL_AND, falseExpr, secondExpr);
    exp.evaluate(&econtext);

    ASSERT_FALSE(secondExpr->wasEvaluated);
}

TEST(BinaryExpressionTest, logicalOrShortCircuit)
{
    cish::vm::Memory memory(100, 1);
    cish::vm::ExecutionContext econtext(&memory);

    auto trueExpr = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 1));
    auto secondExpr = std::make_shared<TestExpression>();

    BinaryExpression exp(BinaryExpression::LOGICAL_OR, trueExpr, secondExpr);
    exp.evaluate(&econtext);

    ASSERT_FALSE(secondExpr->wasEvaluated);
}
