#include "ForLoopStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


ForLoopStatement::ForLoopStatement(Statement *init, Expression *cond, Statement *iter):
    _initialization(init),
    _condition(cond),
    _iterator(iter)
{

}

ForLoopStatement::~ForLoopStatement()
{
    if (_initialization)
        delete _initialization;
    if (_condition)
        delete _condition;
    if (_iterator)
        delete _iterator;
}

void ForLoopStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    Statement::execute(context);
    context->pushScope();

    if (_initialization) {
        _initialization->execute(context);
    }

    while (evaluateCondition(context)) {
        SuperStatement::execute(context);
        if (context->currentFunctionHasReturned())
            break;

        Statement::execute(context);
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
}