#pragma once

#include <vector>

#include "StackFrame.h"
#include "Memory.h"
#include "ExecutionThread.h"

#include "../ast/Ast.h"
#include "../ast/SuperStatement.h"
#include "../Exception.h"


namespace cish
{
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

private:
    std::vector<StackFrame*> _stackFrames;
    Memory *_memory;
};


}
}
