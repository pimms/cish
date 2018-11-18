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

void ExecutionContext::copyStringTable(const ast::StringTable *stringTable)
{
    auto map = stringTable->getMap();
    for (const auto& pair: map) {
        const uint32_t allocLen = pair.second.length() + 1;
        Allocation::Ptr alloc = _memory->allocate(allocLen);
        alloc->copy(pair.second.data(), allocLen);
        _stringMap[pair.first] = std::move(alloc);
    }
}

MemoryView ExecutionContext::resolveString(ast::StringId stringId) const
{
    if (_stringMap.count(stringId) == 0) {
        return _memory->getView(0);
    }

    const uint32_t addr = _stringMap.at(stringId)->getAddress();
    return _memory->getView(addr);
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
    _frameStack.push_back(FunctionFrame { {scope}, false, ast::ExpressionValue(0) });
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

void ExecutionContext::returnCurrentFunction(ast::ExpressionValue retval)
{
    if (_frameStack.empty())
        Throw(Exception, "Cannot return from outside a function");
    if (_frameStack.back().hasReturned)
        Throw(Exception, "Current function has already returned");

    _frameStack.back().hasReturned = true;
    _frameStack.back().returnValue = retval;
}

bool ExecutionContext::currentFunctionHasReturned() const
{
    // Keep in mind that this method will get called by ALL statements, including
    // those in the global scope - as such, we need to inform them gently that
    // the current function - albeit undefined - has NOT returned.
    if (_frameStack.empty())
        return false;

    return _frameStack.back().hasReturned;
}

ast::ExpressionValue ExecutionContext::getCurrentFunctionReturnValue() const
{
    if (_frameStack.empty())
        Throw(Exception, "No return value available");

    return _frameStack.back().returnValue;
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

const Callable::Ptr ExecutionContext::getFunctionDefinition(const std::string &funcName) const
{
    // Nothing we can do! This method must be overridden to serve any purpose.
    printf("WARNING! getFunctionDefinition called on ExecutionContext");
    return nullptr;
}


}
}
