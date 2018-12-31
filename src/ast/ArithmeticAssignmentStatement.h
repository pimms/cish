#pragma once

#include "AstNodes.h"
#include "Lvalue.h"
#include "BinaryExpression.h"
#include "MutableLiteralExpression.h"


namespace cish
{
namespace ast
{


class ArithmeticAssignmentStatement : public Statement
{
public:
    ArithmeticAssignmentStatement(Lvalue::Ptr lvalue,
                                  BinaryExpression::Operator op,
                                  Expression::Ptr expr);

    virtual void execute(vm::ExecutionContext*) const;

private:
    Lvalue::Ptr _lvalue;
    BinaryExpression::Operator _operator;
    Expression::Ptr _expression;

    BinaryExpression::Ptr _binaryExpression;
    mutable MutableLiteralExpression::Ptr _leftExpr;
    mutable MutableLiteralExpression::Ptr _rightExpr;


    ExpressionValue getLeftValue(vm::MemoryView &memoryView) const;
    void writeResult(vm::MemoryView &memView, const ExpressionValue &value) const;
};



}
}

