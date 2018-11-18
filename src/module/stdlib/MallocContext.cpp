#include "MallocContext.h"
#include "stdlibModule.h"


namespace cish
{
namespace module
{
namespace stdlib
{



void MallocContext::onAllocation(vm::Allocation::Ptr alloc)
{
    const uint32_t addr = alloc->getAddress();
    if (_allocations.count(addr) != 0) {
        Throw(StdlibException, "address %x already allocated", addr);
    }

    _allocations[addr] = std::move(alloc);
}

bool MallocContext::attemptDeallocation(uint32_t addr)
{
    if (_allocations.count(addr) == 0) {
        return false;
    }

    _allocations.erase(addr);
    return true;
}


}
}
}