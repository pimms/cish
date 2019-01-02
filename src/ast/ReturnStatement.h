#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class FunctionDefinition;
class DeclarationContext;

class ReturnStatement: public Statement
{
public:
    ReturnStatement(DeclarationContext *context, Expression::Ptr expr);

protected:
    virtual void virtualExecute(vm::ExecutionContext *context) const override;

private:
    ExpressionValue getReturnValue(vm::ExecutionContext *context) const;

    Expression::Ptr _expression;
};


}
}
