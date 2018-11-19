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
    typedef std::shared_ptr<IfStatement> Ptr;

    IfStatement(Expression::Ptr expression, ElseStatement::Ptr elseStatement);

    void execute(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _expression;
    ElseStatement::Ptr _elseStatement;
};


}
}
