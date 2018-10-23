#pragma once

#include <stdint.h>

#include "Memory.h"

namespace cish
{
namespace vm
{


struct VmOptions
{
    // The total size of the memory in bytes
    uint32_t heapSize;

    // The smallest supported allocation unit, in bytes. Any allocation
    // attempt of a lower amount of bytes will result in al allocation
    // of 'minAllocSize' bytes.
    uint32_t minAllocSize;
};


class VirtualMachine
{
public:
    VirtualMachine(const VmOptions &opts);
    ~VirtualMachine();

private:
    Memory *_memory;
};


}
}
