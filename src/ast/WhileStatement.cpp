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
WhileStatement::WhileStatement(Expression::Ptr condition):
	_condition(condition)
{

}

void WhileStatement::virtualExecute(vm::ExecutionContext *context) const
{
	context->pushScope();

	while (evaluateCondition(context)) {
		executeChildStatements(context);
		if (context->currentFunctionHasReturned())
			break;
        synchronize(context);
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
DoWhileStatement::DoWhileStatement(Expression::Ptr condition)
	: WhileStatement(condition) {}

void DoWhileStatement::virtualExecute(vm::ExecutionContext *context) const
{
	context->pushScope();

	do {
		executeChildStatements(context);
		if (context->currentFunctionHasReturned())
			break;
        synchronize(context);
	} while (evaluateCondition(context));

	context->popScope();
}


}
}
