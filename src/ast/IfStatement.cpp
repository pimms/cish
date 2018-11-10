#include "IfStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


IfStatement::IfStatement(Expression *expression):
    _expression(expression)
{
    if (!expression->getType().castableTo(TypeDecl::BOOL)) {
        Throw(InvalidCastException, "Expression in if-statement not convertible to bool");
    }
}

IfStatement::~IfStatement()
{
    delete _expression;
}

void IfStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    Statement::execute(context);

    context->pushScope();

    if (_expression->evaluate(context).get<bool>()) {
        SuperStatement::execute(context);
    }

    context->popScope();
}


}
}
