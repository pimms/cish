#pragma once

#include <stdint.h>
#include <stdexcept>
#include "AllocationUnit.h"


namespace cish
{
namespace vm
{


class OutOfMemoryException : public std::runtime_error {
public:
    OutOfMemoryException(const char *msg): std::runtime_error(msg) {}
};


class Memory : private DeallocationDelegate
{
public:
    Memory(uint32_t heapSize, uint32_t minAllocSize);
    ~Memory();

    AllocationUnit::Ptr allocate(uint32_t size);

private:
    uint32_t _heapSize;
    uint32_t _allocationSize;

    uint8_t *_heap;
    uint8_t *_allocationMap;

    uint32_t findUnallocatedRun(uint32_t len);
    void markAsAllocated(uint32_t offset, uint32_t len);
    void markAsFree(uint32_t offset, uint32_t len);
    uint32_t bytesToAllocationUnits(uint32_t byteCount) const;

    /* DeallocationDelegate */
    void onDeallocation(AllocationUnit *allocation) override;
};

}
}
