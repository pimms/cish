#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class ExpressionStatement: public Statement
{
public:
    ExpressionStatement(Expression::Ptr expression);

    void execute(vm::ExecutionContext *ctx) const override;

private:
    Expression::Ptr _expression;
};


}
}
