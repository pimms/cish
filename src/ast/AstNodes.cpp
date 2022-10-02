#include "AstNodes.h"
#include "../Exception.h"
#include "../vm/ExecutionContext.h"


namespace cish::ast
{

Statement::~Statement()
{
}

void Statement::execute(vm::ExecutionContext *context) const
{
    if (!context->currentFunctionHasReturned()) {
        _ephemeralVariables.push(std::vector<std::unique_ptr<vm::Variable>>());

        synchronize(context);
        virtualExecute(context);
        desynchronize(context);

        _ephemeralVariables.pop();
    }
}

vm::Variable* Statement::allocateEphemeral(vm::ExecutionContext *context, TypeDecl type) const
{
    vm::Allocation::Ptr alloc = context->getMemory()->allocate(type.getSize());

    auto var = std::make_unique<vm::Variable>(type, std::move(alloc));
    auto raw = var.get();
    _ephemeralVariables.top().push_back(std::move(var));

    return raw;
}

void Statement::synchronize(vm::ExecutionContext *context) const
{
    context->onStatementEnter(this);
}

void Statement::desynchronize(vm::ExecutionContext *context) const
{
    context->onStatementExit(this);
}

void NoOpStatement::virtualExecute(vm::ExecutionContext *context) const
{
	Statement::execute(context);
}

}
