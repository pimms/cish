#pragma once

#include <vector>

#include "Scope.h"
#include "Memory.h"
#include "ExecutionThread.h"

#include "../Exception.h"


namespace cish
{
namespace ast
{

class FunctionDefinition;
class Statement;

}

namespace vm
{


DECLARE_EXCEPTION(StackUnderflowException);
DECLARE_EXCEPTION(StackOverflowException);
DECLARE_EXCEPTION(NoEntryPointException);


class ExecutionContext
{
public:
    ExecutionContext(Memory *memory);
    virtual ~ExecutionContext();

    void pushScope();
    void popScope();

    void pushFunctionFrame();
    void popFunctionFrame();

    Scope* getScope() const;
    Memory* getMemory() const;

    virtual void yieldOnStatement(const ast::Statement *statement);
    virtual const ast::FunctionDefinition* getFunctionDefinition(const std::string &funcName) const;

private:
    struct FunctionFrame
    {
        std::vector<Scope*> scopes;
    };
    Scope *_globalScope;
    std::vector<FunctionFrame> _frameStack;

    Memory *_memory;
};


}
}
