#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"
#include "ElseStatement.h"

namespace cish::ast
{

class IfStatement: public SuperStatement
{
public:
    typedef std::shared_ptr<IfStatement> Ptr;

    IfStatement(Expression::Ptr expression, ElseStatement::Ptr elseStatement);

protected:
    void virtualExecute(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _expression;
    ElseStatement::Ptr _elseStatement;
};

}
