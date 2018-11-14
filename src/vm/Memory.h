#pragma once

#include "Allocation.h"
#include "../Exception.h"

#include <stdint.h>
#include <stdexcept>
#include <vector>


namespace cish
{
namespace vm
{


DECLARE_EXCEPTION(OutOfMemoryException);
DECLARE_EXCEPTION(InvalidReadException);


class Memory : private MemoryAccess
{
public:
    static uint32_t firstUsableMemoryAddress();

    Memory(uint32_t heapSize, uint32_t minAllocSize);
    virtual ~Memory();

    uint32_t getTotalSize() const;
    uint32_t getFreeSize() const;

    Allocation::Ptr allocate(uint32_t size);

    std::vector<uint8_t> safeRead(uint32_t addr, uint32_t len);

private:
    const uint32_t _heapSize;
    const uint32_t _allocationSize;
    const uint32_t _numAllocationUnits;
    uint8_t *_heap;
    uint8_t *_allocationMap;

    uint32_t findUnallocatedRun(uint32_t len);
    void markAsAllocated(uint32_t offset, uint32_t len);
    void markAsFree(uint32_t offset, uint32_t len);
    bool isUnitAllocated(uint32_t unitIndex) const;
    uint32_t bytesToAllocations(uint32_t byteCount) const;

    /* MemoryAccess */
    uint8_t* resolve(uint32_t address) override;
    void onDeallocation(Allocation *allocation) override;
};

}
}
