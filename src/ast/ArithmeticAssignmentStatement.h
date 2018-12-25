#pragma once

#include "AstNodes.h"
#include "Lvalue.h"
#include "BinaryExpression.h"


namespace cish
{
namespace ast
{


class ArithmeticAssignmentStatement : public Statement
{
public:
    ArithmeticAssignmentStatement(Lvalue::Ptr lvalue, BinaryExpression::Operator op, Expression::Ptr expr);

    virtual void execute(vm::ExecutionContext*) const;

private:
    Lvalue::Ptr _lvalue;
    BinaryExpression::Operator _operator;
    Expression::Ptr _expression;


    void getOperands(vm::ExecutionContext *context,
                     ExpressionValue &outLeft,
                     ExpressionValue &outRight) const;
    void writeResult(vm::MemoryView &memView, const ExpressionValue &value) const;
};



}
}

