#include "ExpressionStatement.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


ExpressionStatement::ExpressionStatement(Expression::Ptr expression):
    _expression(expression)
{ }

void ExpressionStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    Statement::execute(context);
    _expression->evaluate(context);
}


}
}
