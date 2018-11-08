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
    _globalScope = new Scope();
}

ExecutionContext::~ExecutionContext()
{
    for (FunctionFrame &functionFrame: _frameStack) {
        for (Scope *scope: functionFrame.scopes) {
            delete scope;
        }
    }

    delete _globalScope;
}


void ExecutionContext::pushScope()
{
    if (_frameStack.empty()) {
        Throw(Exception, "Cannot push a scope without a function frame");
    }

    Scope *scope = new Scope(getScope());
    _frameStack.back().scopes.push_back(scope);
}

void ExecutionContext::popScope()
{
    if (_frameStack.empty()) {
        Throw(Exception, "Cannot push a scope without a function frame");
    }

    if (_frameStack.back().scopes.size() == 1) {
        Throw(StackUnderflowException, "Cannot pop the root scope");
    }

    delete _frameStack.back().scopes.back();
    _frameStack.back().scopes.pop_back();
}

void ExecutionContext::pushFunctionFrame()
{
    if (_frameStack.size() > MAX_STACK_FRAMES) {
        Throw(StackOverflowException, "Call stack exceeded maximum limit of %d", MAX_STACK_FRAMES);
    }

    Scope *scope = new Scope(_globalScope);
    _frameStack.push_back(FunctionFrame { {scope} });
}

void ExecutionContext::popFunctionFrame()
{
    if (_frameStack.empty()) {
        Throw(StackUnderflowException, "No function frame to pop");
    }

    if (_frameStack.back().scopes.size() != 1) {
        Throw(Exception, "Illegal operation - cannot pop function frame without popping all scopes first");
    }

    delete _frameStack.back().scopes[0];
    _frameStack.pop_back();
}

Scope* ExecutionContext::getScope() const
{
    if (_frameStack.empty())
        return _globalScope;

    return _frameStack.back().scopes.back();
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
    // Nothing we can do! This method must be overridden to serve any purpose.
    printf("WARNING! getFunctionDefinition called on ExecutionContext");
    return nullptr;
}


}
}
