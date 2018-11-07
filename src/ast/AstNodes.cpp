#include "AstNodes.h"
#include "../Exception.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


void Statement::execute(vm::ExecutionContext *context) const
{
    context->yieldOnStatement(this);
}


}
}
