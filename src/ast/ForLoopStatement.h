#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class ForLoopStatement: public SuperStatement
{
public:
    ForLoopStatement(Statement *init,
                     Expression *condition,
                     Statement *iter);
    ~ForLoopStatement();

    void execute(vm::ExecutionContext *context) const override;

private:
    bool evaluateCondition(vm::ExecutionContext *context) const;

    Statement *_initialization;
    Expression *_condition;
    Statement *_iterator;
};


}
}