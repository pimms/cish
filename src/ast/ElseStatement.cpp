#include "ElseStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


void ElseStatement::execute(vm::ExecutionContext *context) const
{
	if (context->currentFunctionHasReturned())
		return;
	Statement::execute(context);
	SuperStatement::execute(context);
}


}
}
