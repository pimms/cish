#include "VirtualMachine.h"

namespace cish
{
    
using ast::Ast;

namespace vm
{
    

VirtualMachine::VirtualMachine(const VmOptions &opts, Ast::Ptr ast):
    _memory(new Memory(opts.heapSize, opts.minAllocSize)),
    _executionContext(new ExecutionContext(_memory)),
    _ast(std::move(ast)),
    _nextStatementIdx(0)
{ }

VirtualMachine::~VirtualMachine()
{
    delete _executionContext;
    delete _memory;
}
    
const ExecutionContext* VirtualMachine::getExecutionContext() const
{
    return _executionContext;
}

void VirtualMachine::executeNextStatement()
{
    auto rootStatements = _ast->getRootStatements();
    if (_nextStatementIdx < rootStatements.size()) {
        rootStatements[_nextStatementIdx]->execute(_executionContext);
        _nextStatementIdx++;
    }
}


}
}
