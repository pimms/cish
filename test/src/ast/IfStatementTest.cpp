#include <gtest/gtest.h>

#include "ast/IfStatement.h"
#include "ast/LiteralExpression.h"
#include "ast/ExpressionValue.h"


using namespace cish::vm;
using namespace cish::ast;


TEST(IfStatementTest, expressionMustBeConvertibleToBool)
{
    ExpressionValue value(TypeDecl(TypeDecl::VOID), 0);
    LiteralExpression expr(value);
    ASSERT_THROW(IfStatement stmt(&expr), InvalidCastException);
}
