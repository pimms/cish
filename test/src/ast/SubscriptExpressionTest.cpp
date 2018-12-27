#include <gtest/gtest.h>

#include "ast/Lvalue.h"
#include "ast/LiteralExpression.h"

#include "vm/Memory.h"
#include "vm/ExecutionContext.h"


using namespace cish::ast;
using namespace cish::vm;


TEST(SubscriptExpressionTest, expectedStride)
{
    const std::vector<std::pair<TypeDecl,uint32_t>> sizeMap = {
        {TypeDecl::VOID, 1},
        {TypeDecl::BOOL, 1},
        {TypeDecl::CHAR, 1},
        {TypeDecl::SHORT, 2},
        {TypeDecl::INT, 4},
        {TypeDecl::FLOAT, 4},
        {TypeDecl::DOUBLE, 8},
        {TypeDecl::getPointer(TypeDecl::VOID), 4},
    };
    const std::vector<TypeDecl> indexTypes = {
        TypeDecl::BOOL,
        TypeDecl::CHAR,
        TypeDecl::SHORT,
        TypeDecl::INT,
    };
    const std::vector<int32_t> indices = { 0, 1, 10, -1, -10 };

    Memory memory(1024, 4);
    ExecutionContext ec(&memory);

    for (std::pair<TypeDecl,uint32_t> sizePair: sizeMap) {
        for (TypeDecl indexType: indexTypes) {
            for (int32_t index: indices) {
                const uint32_t initialPtrVal = 0x1000;

                ExpressionValue ptrVal(TypeDecl::getPointer(sizePair.first), initialPtrVal);
                ExpressionValue idxVal(indexType, index);

                auto ptrExpr = std::make_shared<LiteralExpression>(ptrVal);
                auto idxExpr = std::make_shared<LiteralExpression>(idxVal);

                auto subscript = SubscriptExpression(ptrExpr, idxExpr);

                MemoryView view = subscript.getMemoryView(&ec);
                EXPECT_EQ(initialPtrVal + (index * sizePair.second), view.getAddress());
            }
        }
    }
}

TEST(SubscriptExpressionTest, floatsCannotBeUsedForIndexing)
{
    ExpressionValue ptrVal(TypeDecl::getPointer(TypeDecl::INT), 0x1000);
    ExpressionValue idxVal(TypeDecl::FLOAT, 1.f);

    auto ptrExpr = std::make_shared<LiteralExpression>(ptrVal);
    auto idxExpr = std::make_shared<LiteralExpression>(idxVal);

    ASSERT_THROW(std::make_shared<SubscriptExpression>(ptrExpr, idxExpr), InvalidOperationException);
}

TEST(SubscriptExpressionTest, doublesCannotBeUsedForIndexing)
{
    ExpressionValue ptrVal(TypeDecl::getPointer(TypeDecl::INT), 0x1000);
    ExpressionValue idxVal(TypeDecl::DOUBLE, 1.f);

    auto ptrExpr = std::make_shared<LiteralExpression>(ptrVal);
    auto idxExpr = std::make_shared<LiteralExpression>(idxVal);

    ASSERT_THROW(std::make_shared<SubscriptExpression>(ptrExpr, idxExpr), InvalidOperationException);
}

TEST(SubscriptExpressionTest, pointersCannotBeUsedForIndexing)
{
    ExpressionValue ptrVal(TypeDecl::getPointer(TypeDecl::INT), 0x1000);
    ExpressionValue idxVal(TypeDecl::getPointer(TypeDecl::INT), 0x1039);

    auto ptrExpr = std::make_shared<LiteralExpression>(ptrVal);
    auto idxExpr = std::make_shared<LiteralExpression>(idxVal);

    ASSERT_THROW(std::make_shared<SubscriptExpression>(ptrExpr, idxExpr), InvalidOperationException);
}
