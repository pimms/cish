#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"
#include "ElseStatement.h"


namespace cish
{
namespace ast
{


class IfStatement: public SuperStatement
{
public:
    IfStatement(Expression *expression, ElseStatement *elseStatement);
    virtual ~IfStatement();

    void execute(vm::ExecutionContext *context) const override;

private:
    Expression *_expression;
    ElseStatement *_elseStatement;
};


}
}
