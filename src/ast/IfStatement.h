#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class IfStatement: public SuperStatement
{
public:
    IfStatement(Expression *expression);
    virtual ~IfStatement();

    void execute(vm::ExecutionContext *context) const override;

private:
    Expression *_expression;
};


}
}
