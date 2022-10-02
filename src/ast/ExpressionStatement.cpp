#include "ExpressionStatement.h"
#include "../vm/ExecutionContext.h"


namespace cish::ast
{

ExpressionStatement::ExpressionStatement(Expression::Ptr expression):
    _expression(expression)
{ }

void ExpressionStatement::virtualExecute(vm::ExecutionContext *context) const
{
    _expression->evaluate(context);
}

}
