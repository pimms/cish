#include "ExecutionContext.h"
#include "StdoutStream.h"
#include "../ast/FunctionDefinition.h"
#include "../ast/AstNodes.h"


namespace cish
{
namespace vm
{

// If this ever occurs, something fucky is definitely going on
const int MAX_STACK_FRAMES = 4096;



ExecutionContext::ExecutionContext(Memory *memory):
    _memory(memory),
    _customStdout(nullptr),
    _defaultStdout(new StdoutStream())
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
    delete _defaultStdout;
}

void ExecutionContext::copyStringTable(const ast::StringTable *stringTable)
{
    auto map = stringTable->getMap();
    for (const auto& pair: map) {
        const uint32_t allocLen = pair.second.length() + 1;
        Allocation::Ptr alloc = _memory->allocate(allocLen);
        alloc->writeBuf(pair.second.data(), allocLen);
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
    _frameStack.push_back(FunctionFrame { {scope}, false, ast::ExpressionValue(0), nullptr });
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

void ExecutionContext::setFunctionReturnBuffer(vm::Variable *buffer)
{
    if (_frameStack.empty())
        Throw(Exception, "No function frame");
    if (_frameStack.back().returnBuffer != nullptr)
        Throw(Exception, "Current frame already has a return buffer");
    _frameStack.back().returnBuffer = buffer;
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

vm::Variable* ExecutionContext::getCurrentFunctionReturnBuffer() const
{
    if (_frameStack.empty())
        Throw(Exception, "No function frames on stack");
    if (_frameStack.back().returnBuffer == nullptr)
        Throw(Exception, "No return buffer defined for current function");

    return _frameStack.back().returnBuffer;
}

const ast::Statement* ExecutionContext::getCurrentStatement() const
{
    if (_statementStack.empty())
        return nullptr;
    return _statementStack.top();
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

void ExecutionContext::onStatementEnter(const ast::Statement *statement)
{
    if (!statement)
        return;
    if (!_statementStack.empty() && _statementStack.top() == statement)
        return;

    _statementStack.push(statement);
}

void ExecutionContext::onStatementExit(const ast::Statement *statement)
{
    if (_statementStack.empty())
        Throw(Exception, "Statement-stack is empty");
    if (_statementStack.top() != statement)
        Throw(Exception, "Last statement is not the exited one");
    _statementStack.pop();
}

const Callable::Ptr ExecutionContext::getFunctionDefinition(const std::string &funcName) const
{
    // Nothing we can do! This method must be overridden to serve any purpose.
    fprintf(stderr, "WARNING! getFunctionDefinition called on ExecutionContext");
    return nullptr;
}

void ExecutionContext::setStdout(IStream *stream)
{
    _customStdout = stream;
}

IStream* ExecutionContext::getStdout()
{
    return _customStdout ? _customStdout : _defaultStdout;
}


}
}
