#include "ExpressionStatement.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


ExpressionStatement::ExpressionStatement(Expression *expression):
    _expression(expression)
{ }

ExpressionStatement::~ExpressionStatement()
{
    delete _expression;
}

void ExpressionStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    Statement::execute(context);
    _expression->evaluate(context);
}


}
}
