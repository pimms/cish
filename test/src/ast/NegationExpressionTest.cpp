#include <gtest/gtest.h>
#include <vector>

#include "ast/NegationExpression.h"
#include "ast/LiteralExpression.h"
#include "vm/Memory.h"
#include "vm/ExecutionContext.h"

using namespace cish::vm;
using namespace cish::ast;


TEST(NegationExpressionTest, negationOfAllTypes)
{
    std::vector<ExpressionValue> values = {
        ExpressionValue(TypeDecl::BOOL, true),
        ExpressionValue(TypeDecl::BOOL, false),

        ExpressionValue(TypeDecl::CHAR, 0),
        ExpressionValue(TypeDecl::CHAR, 120),
        ExpressionValue(TypeDecl::CHAR, -120),

        ExpressionValue(TypeDecl::SHORT, 0),
        ExpressionValue(TypeDecl::SHORT, -100),

        ExpressionValue(TypeDecl::INT, 0),
        ExpressionValue(TypeDecl::INT, 100),
        ExpressionValue(TypeDecl::INT, -100),

        ExpressionValue(TypeDecl::FLOAT, 0.f),
        ExpressionValue(TypeDecl::FLOAT, 100.f),
        ExpressionValue(TypeDecl::FLOAT, -100.f),

        ExpressionValue(TypeDecl::DOUBLE, 0.0),
        ExpressionValue(TypeDecl::DOUBLE, 100.0),
        ExpressionValue(TypeDecl::DOUBLE, -100.0),

        ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 0),
        ExpressionValue(TypeDecl::getPointer(TypeDecl::INT), 0x00400082),
    };

    Memory memory(100, 1);
    ExecutionContext ec(&memory);

    for (auto val: values) {
        const bool unNegated = val.get<bool>();
        const bool negated = !unNegated;

        auto literal = new LiteralExpression(val);
        NegationExpression negateExpr(literal);

        ASSERT_EQ(TypeDecl::BOOL, negateExpr.getType().getType());
        ASSERT_EQ(negated, negateExpr.evaluate(&ec).get<bool>());
    }

}