#include "ReturnStatement.h"

#include "DeclarationContext.h"
#include "FunctionDefinition.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


ReturnStatement::ReturnStatement(DeclarationContext *context, Expression::Ptr expr):
    _expression(expr)
{
    const FunctionDefinition::Ptr functionDefinition = context->getCurrentFunction();
    if (functionDefinition == nullptr) {
        Throw(InvalidStatementException, "Cannot return outside of a function");
    }

    const auto returnType = functionDefinition->getDeclaration()->returnType;

    if (expr == nullptr) {
        if (returnType.getType() != TypeDecl::VOID) {
            Throw(InvalidTypeException, "Cannot return without value from non-void function");
        }
    } else {
        const auto actualType = expr->getType();

        if (actualType == TypeDecl(TypeDecl::VOID)) {
            Throw(InvalidCastException, "Cannot return void");
        }
        if (!actualType.castableTo(returnType)) {
            Throw(InvalidCastException, "Cannot convert type of value '%s' to expected return type '%s'",
                    actualType.getName(), returnType.getName());
        }
    }
}

void ReturnStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;

    Statement::execute(context);

    if (_expression) {
        context->returnCurrentFunction(_expression->evaluate(context));
    } else {
        // TODO: Find a better way to handle void
        context->returnCurrentFunction(ExpressionValue(0));
    }
}


}
}
