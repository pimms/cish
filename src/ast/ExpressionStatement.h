#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class ExpressionStatement: public Statement
{
public:
    ExpressionStatement(Expression *expression);
    ~ExpressionStatement();

    void execute(vm::ExecutionContext *ctx) const override;

private:
    Expression *_expression;
};


}
}
