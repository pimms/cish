#include "ExecutionContext.h"

namespace cish
{
namespace vm
{

// If this ever occurs, something fucky is definitely going on
const int MAX_STACK_FRAMES = 100;


ExecutionContext::ExecutionContext(Memory *memory):
    _memory(memory)
{
    _stackFrames.push_back(new StackFrame());
}

ExecutionContext::~ExecutionContext()
{
    for (StackFrame *frame: _stackFrames) {
        delete frame;
    }
}


void ExecutionContext::pushStackFrame()
{
    if (_stackFrames.size() >= MAX_STACK_FRAMES) {
        Throw(StackOverflowException, "Too many stack-frames (%d)", _stackFrames.size());
    }

    StackFrame *frame = new StackFrame(getStackFrame());
    _stackFrames.push_back(frame);
}

void ExecutionContext::popStackFrame()
{
    if (_stackFrames.size() == 1) {
        Throw(StackUnderflowException, "Cannot pop the root stack-frame");
    }

    delete _stackFrames.back();
    _stackFrames.pop_back();
}

StackFrame* ExecutionContext::getStackFrame() const
{
    return _stackFrames.back();
}

Memory* ExecutionContext::getMemory() const
{
    return _memory;
}


}
}
