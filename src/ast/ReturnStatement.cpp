#include "ReturnStatement.h"

#include "DeclarationContext.h"
#include "FunctionDefinition.h"


namespace cish
{
namespace ast
{


ReturnStatement::ReturnStatement(DeclarationContext *context, Expression *expr):
    _expression(expr),
    _functionDefinition(context->getCurrentFunction())
{
    if (_functionDefinition == nullptr) {
        Throw(InvalidStatementException, "Cannot return outside of a function");
    }

    auto actualType = expr->getType();
    auto returnType = _functionDefinition->getDeclaration()->returnType;
    if (!actualType.castableTo(returnType)) {
        Throw(InvalidCastException, "Cannot convert type of value '%s' to expected return type '%s'",
                actualType.getName(), returnType.getName());
    }
}

void ReturnStatement::execute(vm::ExecutionContext *context) const
{
    Statement::execute(context);
}


}
}
