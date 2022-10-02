#include "ElseStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish::ast
{

void ElseStatement::virtualExecute(vm::ExecutionContext *context) const
{
	executeChildStatements(context);
}

}
