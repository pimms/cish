#include <gtest/gtest.h>

#include "ast/VariableReferenceExpression.h"
#include "ast/DeclarationContext.h"

using namespace cish::ast;
using namespace cish::vm;


TEST(VariableReferenceExpressionTest, undeclaredVariableReferenceThrows)
{
    DeclarationContext dc;
    ASSERT_THROW(VariableReferenceExpression(&dc, "var"), VariableNotDeclaredException);
}

TEST(VariableReferenceExpressionTest, declaredVariableReferenceDoesNotThrow)
{
    DeclarationContext dc;
    dc.declareVariable("var", TypeDecl::INT);
    ASSERT_NO_THROW(VariableReferenceExpression(&dc, "var"));
}

TEST(VariableReferenceExpressionTest, declaredVariableTypeIsInheritedByExpression)
{
    DeclarationContext dc;
    dc.declareVariable("intVar", TypeDecl::INT);
    dc.declareVariable("floatVar", TypeDecl::FLOAT);

    VariableReferenceExpression iexpr(&dc, "intVar");
    ASSERT_EQ(TypeDecl(TypeDecl::INT), iexpr.getType());

    VariableReferenceExpression fexpr(&dc, "floatVar");
    ASSERT_EQ(TypeDecl(TypeDecl::FLOAT), fexpr.getType());
}




