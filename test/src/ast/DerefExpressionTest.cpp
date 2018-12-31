#include <gtest/gtest.h>

#include "ast/Lvalue.h"
#include "ast/LiteralExpression.h"
#include "ast/IncdecExpression.h"

#include "vm/ExecutionContext.h"
#include "vm/Memory.h"
#include "vm/Variable.h"

#include <memory>

using namespace cish::vm;
using namespace cish::ast;


TEST(DereferenceExpressionTest, simpleDereference)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    TypeDecl type = TypeDecl::getPointer(TypeDecl::INT);
    dc.declareVariable(type, "ptr");

    Variable *var = new Variable(type, memory.allocate(4));
    var->getAllocation()->write<int>(15);
    ec.getScope()->addVariable("ptr", var);

    ExpressionValue exprVal(type, var->getAllocation()->getAddress());
    auto literal = std::make_shared<LiteralExpression>(exprVal);

    DereferenceExpression deref(literal);
    ASSERT_EQ(TypeDecl::INT, deref.getType().getType());
    ASSERT_EQ(15, deref.evaluate(&ec).get<int>());
}

TEST(DereferenceExpressionTest, testWithIncrementalOperand)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    TypeDecl type = TypeDecl::getPointer(TypeDecl::INT);
    dc.declareVariable(type, "ptr");

    Variable *rawVal = new Variable(TypeDecl::INT, memory.allocate(4));
    rawVal->getAllocation()->write<int>(15);

    Variable *ptrVar = new Variable(type, memory.allocate(4));
    ptrVar->getAllocation()->write<int>(rawVal->getAllocation()->getAddress());
    ec.getScope()->addVariable("ptr", ptrVar);

    auto lvalue = std::make_shared<VariableReference>(&dc, "ptr");
    auto inc = std::make_shared<IncDecExpression>(IncDecExpression::POSTFIX_INCREMENT, lvalue);

    DereferenceExpression deref(inc);
    ASSERT_EQ(TypeDecl::INT, deref.getType().getType());
    ASSERT_EQ(15, deref.evaluate(&ec).get<int>());

    delete rawVal;
}
