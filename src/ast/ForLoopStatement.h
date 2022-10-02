#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"


namespace cish::ast
{

class ForLoopStatement: public SuperStatement
{
public:
    typedef std::shared_ptr<ForLoopStatement> Ptr;

    ForLoopStatement(Statement::Ptr init,
                     Expression::Ptr condition,
                     Statement::Ptr iter);

protected:
    void virtualExecute(vm::ExecutionContext *context) const override;

private:
    bool evaluateCondition(vm::ExecutionContext *context) const;

    Statement::Ptr _initialization;
    Expression::Ptr _condition;
    Statement::Ptr _iterator;
};

}
