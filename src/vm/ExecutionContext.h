#pragma once

#include <vector>

#include "StackFrame.h"
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

    void pushStackFrame();
    void popStackFrame();

    StackFrame* getStackFrame() const;
    Memory* getMemory() const;

    virtual void yieldOnStatement(const ast::Statement *statement);
    virtual const ast::FunctionDefinition* getFunctionDefinition(const std::string &funcName) const;

private:
    std::vector<StackFrame*> _stackFrames;
    Memory *_memory;
};


}
}
