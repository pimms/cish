#pragma once

#include "AstNodes.h"
#include "FunctionCallExpression.h"
#include "DeclarationContext.h"


namespace cish
{
namespace ast
{


class FunctionCallStatement: public Statement
{
public:
    FunctionCallStatement(DeclarationContext *context, FunctionCallExpression *expr);
    virtual void execute(vm::ExecutionContext*) const override;

private:
    FunctionCallExpression *_expr;
};


}
}
