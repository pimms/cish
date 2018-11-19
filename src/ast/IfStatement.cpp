#include "IfStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


IfStatement::IfStatement(Expression::Ptr expression, ElseStatement::Ptr elseStatement):
    _expression(expression),
    _elseStatement(elseStatement)
{
    if (!expression->getType().castableTo(TypeDecl::BOOL)) {
        Throw(InvalidCastException, "Expression in if-statement not convertible to bool");
    }
}

void IfStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    Statement::execute(context);

    context->pushScope();

    if (_expression->evaluate(context).get<bool>()) {
        SuperStatement::execute(context);
    } else if (_elseStatement) {
        _elseStatement->execute(context);
    }

    context->popScope();
}


}
}
