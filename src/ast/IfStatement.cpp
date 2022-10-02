#include "IfStatement.h"

#include "../vm/ExecutionContext.h"

namespace cish::ast
{

IfStatement::IfStatement(Expression::Ptr expression, ElseStatement::Ptr elseStatement):
    _expression(expression),
    _elseStatement(elseStatement)
{
    if (!expression->getType().castableTo(TypeDecl::BOOL)) {
        Throw(InvalidCastException, "Expression in if-statement not convertible to bool");
    }
}

void IfStatement::virtualExecute(vm::ExecutionContext *context) const
{
    context->pushScope();

    if (_expression->evaluate(context).get<bool>()) {
        executeChildStatements(context);
    } else if (_elseStatement) {
        _elseStatement->execute(context);
    }

    context->popScope();
}

}
