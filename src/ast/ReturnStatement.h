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

    virtual void execute(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _expression;
};


}
}
