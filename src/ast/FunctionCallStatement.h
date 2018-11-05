#pragma once

#include "AstNodes.h"
#include "FunctionCallExpression.h"


namespace cish
{
namespace ast
{


class FunctionCallStatement: public Statement
{
public:
    FunctionCallStatement(FunctionCallExpression *expr);
    virtual void execute(vm::ExecutionContext*) override;

private:
    FunctionCallExpression *_expr;
};


}
}
