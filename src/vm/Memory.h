#pragma once

#include "Allocation.h"
#include "../Exception.h"

#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <map>


namespace cish
{
namespace vm
{


DECLARE_EXCEPTION(OutOfMemoryException);
DECLARE_EXCEPTION(InvalidReadException);
DECLARE_EXCEPTION(InvalidFreeException);
DECLARE_EXCEPTION(InvalidAccessException);


class Memory : private MemoryAccess
{
public:
    static uint32_t firstUsableMemoryAddress();

    Memory(uint32_t heapSize, uint32_t minAllocSize);
    virtual ~Memory();

    uint32_t getTotalSize() const;
    uint32_t getFreeSize() const;

    /**
     * If an allocation can be made, this method guarantees a
     * safe view into memory. The memory will remain safely
     * accessible until the Allocation is deallocated.
     */
    Allocation::Ptr allocate(uint32_t size);

    /**
     * Provides a potentially unsafe view into the memory. No
     * checks are performed to ensure that 'address' is a valid
     * memory address until the MemoryView attempts to perform
     * any accesses into the memory. This method therefore never
     * throws.
     */
    MemoryView getView(uint32_t address) noexcept;

private:
    const uint32_t _heapSize;
    const uint32_t _allocationSize;
    const uint32_t _numAllocationUnits;
    uint8_t *_heap;
    uint8_t *_allocationMap;
    std::map<Allocation*,uint32_t> _allocLen;

    uint32_t findUnallocatedRun(uint32_t len);
    void markAsAllocated(uint32_t offset, uint32_t len);
    void markAsFree(uint32_t offset, uint32_t len);
    bool isUnitAllocated(uint32_t unitIndex) const;
    uint32_t byteOffsetToUnit(uint32_t byteOffset) const;
    uint32_t byteCountToUnitCount(uint32_t byteCount) const;

    /* MemoryAccess */
    void onDeallocation(Allocation *allocation) override;
    const uint8_t* read(uint32_t address, uint32_t len) override;
    void write(const uint8_t *buffer, uint32_t address, uint32_t len) override;
};

}
}
