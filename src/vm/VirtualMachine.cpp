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
    _executor(new Executor(_memory, ast)),
    _started(false)
{
}

VirtualMachine::~VirtualMachine()
{
    _executor->terminate();
    delete _executor;
    delete _memory;
}

ExecutionContext* VirtualMachine::getExecutionContext() const
{
    return _executor;
}

void VirtualMachine::executeBlocking()
{
    if (_started) {
        Throw(VmException, "Cannot restart a started VM");
    }

    _started = true;
    _executor->setWaitForResume(false);
    _executor->runBlocking();
}

void VirtualMachine::startSync()
{
    if (_started) {
        Throw(VmException, "Cannot restart a started VM");
    }

    _started = true;
    _executor->setWaitForResume(true);
    _executor->startAsync();
}

void VirtualMachine::executeNextStatement()
{
    if (_started) {
        if (_executor->isRunning()) {
            _executor->cycle();
        } else {
            Throw(VmException, "Virtual machine not running");
        }
    } else {
        Throw(VmException,
            "Virtual machine not started "
            "(did you forget to call startSync()?)");
    }
}

bool VirtualMachine::hasStarted() const
{
    return _started;
}

bool VirtualMachine::isRunning() const
{
    return _executor->isRunning();
}

int VirtualMachine::getExitCode() const
{
    return _executor->getExitStatus().get<int>();
}

std::shared_ptr<Exception> VirtualMachine::getRuntimeError() const
{
    return _executor->getRuntimeError();
}

void VirtualMachine::terminate()
{
    _executor->terminate();
}


}
}
