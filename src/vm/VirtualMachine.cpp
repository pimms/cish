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
    _globalInitComplete(false),
    _globalStatementIdx(0)
{

}

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
    if (_globalInitComplete) {
        Throw(Exception, "Don't know what to do yet lol");
    } else {
        executeNextGlobalStatement();
    }
}

void VirtualMachine::executeNextGlobalStatement()
{
    auto rootStatements = _ast->getRootStatements();
    if (_globalStatementIdx >= rootStatements.size()) {
        _globalInitComplete = true;
    } else {
        ast::Statement *statement = rootStatements[_globalStatementIdx];
        executeStatement(statement);
        _globalStatementIdx++;
    }
}

void VirtualMachine::executeStatement(const ast::Statement *statement)
{
    statement->execute(_executionContext);
}


}
}
