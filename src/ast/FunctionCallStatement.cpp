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
    _expr->evaluate(context);
}


}
}
