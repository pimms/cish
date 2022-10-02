#include "ForLoopStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish::ast
{

ForLoopStatement::ForLoopStatement(Statement::Ptr init, Expression::Ptr cond, Statement::Ptr iter):
    _initialization(init),
    _condition(cond),
    _iterator(iter)
{

}

void ForLoopStatement::virtualExecute(vm::ExecutionContext *context) const
{
    context->pushScope();
    if (_initialization) {
        _initialization->execute(context);
    }

    while (evaluateCondition(context)) {
        executeChildStatements(context);
        if (context->currentFunctionHasReturned())
            break;

        synchronize(context);
        if (_iterator) {
            _iterator->execute(context);
        }
    }

    context->popScope();
}


bool ForLoopStatement::evaluateCondition(vm::ExecutionContext *context) const
{
    if (!_condition)
        return true;
    return _condition->evaluate(context).get<bool>();
}

}
