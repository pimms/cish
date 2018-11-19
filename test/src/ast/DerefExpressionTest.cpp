#include <gtest/gtest.h>

#include "ast/DerefExpression.h"
#include "ast/LiteralExpression.h"
#include "ast/IncdecExpression.h"

#include "vm/ExecutionContext.h"
#include "vm/Memory.h"
#include "vm/Variable.h"

#include <memory>

using namespace cish::vm;
using namespace cish::ast;


TEST(DerefExpressionTest, simpleDereference)
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

    DerefExpression deref(&dc, literal);
    ASSERT_EQ(TypeDecl::INT, deref.getType().getType());
    ASSERT_EQ(15, deref.evaluate(&ec).get<int>());
}

TEST(DerefExpressionTest, testWithIncrementalOperand)
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

    auto inc = std::make_shared<IncDecExpression>(&dc, IncDecExpression::POSTFIX_INCREMENT, "ptr");

    DerefExpression deref(&dc, inc);
    ASSERT_EQ(TypeDecl::INT, deref.getType().getType());
    ASSERT_EQ(15, deref.evaluate(&ec).get<int>());

    delete rawVal;
}