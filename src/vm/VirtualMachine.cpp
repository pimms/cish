#include "VirtualMachine.h"
#include "Executor.h"
#include "Memory.h"

namespace cish
{

using ast::Ast;

namespace vm
{


VirtualMachine::VirtualMachine(const VmOptions &opts, Ast::Ptr ast):
    _memory(new Memory(opts.heapSize, opts.minAllocSize)),
    _executor(new Executor(_memory, std::move(ast)))
{
    _executor->setWaitForResume(true);
    _executor->start();
}

VirtualMachine::~VirtualMachine()
{
    delete _executor;
    delete _memory;
}

const ExecutionContext* VirtualMachine::getExecutionContext() const
{
    return _executor;
}

void VirtualMachine::executeNextStatement()
{
    if (!_executor->isRunning()) {
        Throw(VmException, "Virtual machine not running");
    }

    _executor->resume();
}


}
}
