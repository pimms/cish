#include "WhileStatement.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{



/*
==============
WhileStatement
==============
*/
WhileStatement::WhileStatement(Expression *condition):
	_condition(condition)
{

}

WhileStatement::~WhileStatement()
{
	delete _condition;
}

void WhileStatement::execute(vm::ExecutionContext *context) const
{
	if (context->currentFunctionHasReturned())
		return;
	Statement::execute(context);
	context->pushScope();

	while (evaluateCondition(context)) {
		SuperStatement::execute(context);
		if (context->currentFunctionHasReturned())
			break;
		Statement::execute(context);
	}

	context->popScope();
}

bool WhileStatement::evaluateCondition(vm::ExecutionContext *context) const
{
	return _condition->evaluate(context).get<bool>();
}



/*
==============
DoWhileStatement
==============
*/
DoWhileStatement::DoWhileStatement(Expression *condition)
	: WhileStatement(condition) {}

void DoWhileStatement::execute(vm::ExecutionContext *context) const
{
	if (context->currentFunctionHasReturned())
		return;
	Statement::execute(context);
	context->pushScope();

	do {
		SuperStatement::execute(context);
		if (context->currentFunctionHasReturned())
			break;
		Statement::execute(context);
	} while (evaluateCondition(context));

	context->popScope();
}


}
}