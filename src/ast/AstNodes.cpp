#include "AstNodes.h"
#include "../Exception.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


Statement::~Statement()
{
    freeEphemeralAllocations();
}

void Statement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
    synchronize(context);
    virtualExecute(context);
    freeEphemeralAllocations();
}

vm::Variable* Statement::allocateEphemeral(vm::ExecutionContext *context, TypeDecl type) const
{
    vm::Allocation::Ptr alloc = context->getMemory()->allocate(type.getSize());

    vm::Variable *var = new vm::Variable(type, std::move(alloc));
    _ephemeralVariables.push_back(var);

    return var;
}

void Statement::synchronize(vm::ExecutionContext *context) const
{
    context->onStatementEnter(this);
}

void Statement::freeEphemeralAllocations() const
{
    for (vm::Variable *var: _ephemeralVariables) {
        delete var;
    }

    _ephemeralVariables.clear();
}



void NoOpStatement::virtualExecute(vm::ExecutionContext *context) const
{
	Statement::execute(context);
}


}
}
