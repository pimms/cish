#include "FunctionCallStatement.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


FunctionCallStatement::FunctionCallStatement(FunctionCallExpression *expr):
    _expr(expr)
{

}

void FunctionCallStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;

    Statement::execute(context);
    _expr->evaluate(context);
}


}
}
