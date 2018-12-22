#include <gtest/gtest.h>

#include "ast/OnesComplementExpression.h"
#include "ast/ExpressionValue.h"
#include "ast/LiteralExpression.h"
#include "vm/Memory.h"
#include "vm/ExecutionContext.h"

using namespace cish::ast;


TEST(OnesComplementExpressionTest, invalidOnFloats)
{
    ExpressionValue value(TypeDecl::FLOAT, 10.f);
    auto literal = std::make_shared<LiteralExpression>(value);
    ASSERT_THROW(OnesComplementExpression expr(literal), InvalidOperationException);
}

TEST(OnesComplementExpressionTest, invalidOnPointers)
{
    ExpressionValue value(TypeDecl::getPointer(TypeDecl::INT) , 0x101010);
    auto literal = std::make_shared<LiteralExpression>(value);
    ASSERT_THROW(OnesComplementExpression expr(literal), InvalidOperationException);
}

TEST(OnesComplementExpressionTest, validOnIntegerTypes)
{
    auto boolVal = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::BOOL, true));
    auto charVal = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::CHAR, 0x11));
    auto shortVal = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::SHORT, 0x1111));
    auto intVal = std::make_shared<LiteralExpression>(ExpressionValue(TypeDecl::INT, 0x11111111));

    cish::vm::Memory memory(100, 4);
    cish::vm::ExecutionContext ec(&memory);
    ASSERT_EQ(0xfffffffe, OnesComplementExpression(boolVal).evaluate(&ec).get<int32_t>());
    ASSERT_EQ(0xffffffee, OnesComplementExpression(charVal).evaluate(&ec).get<int32_t>());
    ASSERT_EQ(0xffffeeee, OnesComplementExpression(shortVal).evaluate(&ec).get<int32_t>());
    ASSERT_EQ(0xeeeeeeee, OnesComplementExpression(intVal).evaluate(&ec).get<int32_t>());
}
