#include <gtest/gtest.h>

#include "ast/FunctionCallExpression.h"
#include "ast/LiteralExpression.h"
#include "ast/FuncDeclaration.h"

using namespace cish::ast;


std::vector<Expression::Ptr> noparams()
{
    return std::vector<Expression::Ptr> {};
}


TEST(FunctionCallExpressionTest, expressionTypeInheritsFromFunction)
{
    FuncDeclaration voidDecl(TypeDecl::VOID, "voidFunc");
    FuncDeclaration intDecl(TypeDecl::INT, "intFunc");

    DeclarationContext context;
    context.declareFunction(voidDecl);
    context.declareFunction(intDecl);

    FunctionCallExpression voidCall(&context, "voidFunc", noparams());
    ASSERT_EQ(TypeDecl(TypeDecl::VOID), voidCall.getType());

    FunctionCallExpression intCall(&context, "intFunc", noparams());
    ASSERT_EQ(TypeDecl(TypeDecl::INT), intCall.getType());
}

TEST(FunctionCallExpressionTest, throwsIfFunctionIsNotDeclared)
{
    DeclarationContext context;
    ASSERT_THROW(FunctionCallExpression expr(&context, "foo", noparams()), FunctionNotDeclaredException);
}

TEST(FunctionCallExpressionTest, tooFewParamsInCallThrows)
{
    FuncDeclaration decl = { TypeDecl::VOID, "foo", { VarDeclaration { TypeDecl::INT, "n" } } };

    DeclarationContext context;
    context.declareFunction(decl);

    ASSERT_THROW(FunctionCallExpression call(&context, "foo", noparams()), InvalidParameterException);
}

TEST(FunctionCallExpressionTest, tooManyParamsInCallThrows)
{
    FuncDeclaration decl(TypeDecl::VOID, "foo");

    DeclarationContext context;
    context.declareFunction(decl);

    std::vector<Expression::Ptr> params = { std::make_shared<LiteralExpression>("100") };
    ASSERT_THROW(FunctionCallExpression call(&context, "foo", params), InvalidParameterException);
}

TEST(FunctionCallExpressionTest, voidExpressionsAsParamThrows)
{
    // This test should really check for incompatible types, but at the time of writing, VOID
    // is the only type that cannot be implicitly converted.
    FuncDeclaration decl(TypeDecl::VOID, "foo", {VarDeclaration { TypeDecl::INT, "n" }});

    DeclarationContext context;
    context.declareFunction(decl);

    // At time of writing I have yet to solve how to handle void ExpressionValues, so the most
    // "future-proof" (tm) way of creating a void-typed expression is to call a void function.
    //
    // Hopefully :-)
    std::vector<Expression::Ptr> params = { std::make_shared<LiteralExpression>("100") };
    auto validCall = std::make_shared<FunctionCallExpression>(&context, "foo", params);

    ASSERT_THROW(FunctionCallExpression invalidCall(&context, "foo", { validCall }), InvalidParameterException);
}

