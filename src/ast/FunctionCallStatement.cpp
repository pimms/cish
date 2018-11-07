#include "FunctionCallStatement.h"


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
    Statement::execute(context);
    _expr->evaluate(context);
}


}
}
