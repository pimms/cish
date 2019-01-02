#include "ElseStatement.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


void ElseStatement::virtualExecute(vm::ExecutionContext *context) const
{
	executeChildStatements(context);
}


}
}
