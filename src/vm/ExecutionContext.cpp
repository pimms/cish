#include "ExecutionContext.h"
#include "../ast/FunctionDefinition.h"
#include "../ast/AstNodes.h"

namespace cish
{
namespace vm
{

// If this ever occurs, something fucky is definitely going on
const int MAX_STACK_FRAMES = 100;


ExecutionContext::ExecutionContext(Memory *memory):
    _memory(memory)
{
    _stackFrames.push_back(new Scope());
}

ExecutionContext::~ExecutionContext()
{
    for (Scope *frame: _stackFrames) {
        delete frame;
    }
}


void ExecutionContext::pushScope()
{
    if (_stackFrames.size() >= MAX_STACK_FRAMES) {
        Throw(StackOverflowException, "Too many stack-frames (%d)", _stackFrames.size());
    }

    Scope *frame = new Scope(getScope());
    _stackFrames.push_back(frame);
}

void ExecutionContext::popScope()
{
    if (_stackFrames.size() == 1) {
        Throw(StackUnderflowException, "Cannot pop the root stack-frame");
    }

    delete _stackFrames.back();
    _stackFrames.pop_back();
}

Scope* ExecutionContext::getScope() const
{
    return _stackFrames.back();
}

Memory* ExecutionContext::getMemory() const
{
    return _memory;
}

void ExecutionContext::yieldOnStatement(const ast::Statement *statement)
{
    // No body! This method must be overriden to serve any purpose.
}

const ast::FunctionDefinition* ExecutionContext::getFunctionDefinition(const std::string &funcName) const
{
    // Nothing we can do! This method must be overriden to serve any purpose.
    printf("WARNING! getFunctionDefinition called on ExecutionContext");
    return nullptr;
}


}
}
