#pragma once

#include <vector>

#include "StackFrame.h"
#include "Memory.h"
#include "../Exception.h"


namespace cish
{
namespace vm
{


DECLARE_EXCEPTION(StackUnderflowException);
DECLARE_EXCEPTION(StackOverflowException);


class ExecutionContext
{
public:
    ExecutionContext(Memory *memory);
    ~ExecutionContext();

    void pushStackFrame();
    void popStackFrame();
    StackFrame* getStackFrame() const;

    Memory* getMemory() const;

private:
    std::vector<StackFrame*> _stackFrames;
    Memory *_memory;
};


}
}