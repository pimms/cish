#include "VirtualMachine.h"

namespace cish
{
namespace vm
{

VirtualMachine::VirtualMachine(const VmOptions &opts)
{
    _memory = new Memory(opts.heapSize, opts.minAllocSize);
}

VirtualMachine::~VirtualMachine()
{
    delete _memory;
}

}
}
