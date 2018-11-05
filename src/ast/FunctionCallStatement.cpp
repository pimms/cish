#include "FunctionCallStatement.h"


namespace cish
{
namespace ast
{


FunctionCallStatement::FunctionCallStatement(DeclarationContext *context, FunctionCallExpression *expr):
    Statement(context->getCurrentSuper()),
    _expr(expr)
{

}

void FunctionCallStatement::execute(vm::ExecutionContext *context) const
{
    _expr->evaluate(context);
}


}
}
