#pragma once

#include <stdint.h>
#include <stdexcept>
#include "Allocation.h"


namespace cish
{
namespace vm
{


class OutOfMemoryException : public std::runtime_error {
public:
    OutOfMemoryException(const char *msg): std::runtime_error(msg) {}
};


class Memory : private MemoryAccess
{
public:
    Memory(uint32_t heapSize, uint32_t minAllocSize);
    ~Memory();

    uint32_t getTotalSize() const;
    uint32_t getFreeSize() const;

    Allocation::Ptr allocate(uint32_t size);

private:
    const uint32_t _heapSize;
    const uint32_t _allocationSize;
    const uint32_t _numAllocationUnits;
    uint8_t *_heap;
    uint8_t *_allocationMap;

    uint32_t findUnallocatedRun(uint32_t len);
    void markAsAllocated(uint32_t offset, uint32_t len);
    void markAsFree(uint32_t offset, uint32_t len);
    uint32_t bytesToAllocations(uint32_t byteCount) const;

    /* MemoryAccess */
    uint8_t* getHeap() override;
    void onDeallocation(Allocation *allocation) override;
};

}
}
