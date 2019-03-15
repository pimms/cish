#include "Memory.h"

#include "../Exception.h"

#include <cassert>


namespace cish
{
namespace vm
{

static const uint32_t FIRST_USABLE_ADDRESS = 0x00400000;

uint32_t Memory::firstUsableMemoryAddress()
{
    return FIRST_USABLE_ADDRESS;
}


Memory::Memory(uint32_t heapSize, uint32_t minAllocSize):
    _heapSize(heapSize),
    _allocationSize(minAllocSize),
    _numAllocationUnits(heapSize / minAllocSize),
    _allocator(_numAllocationUnits)
{
    assert(_heapSize >= 0);
    assert(_allocationSize >= 0);

    _heap = new uint8_t[_heapSize];

    const uint32_t allocMapSize = _heapSize / (_allocationSize * 8) + 1;
    _allocationMap = new uint8_t[allocMapSize];
    memset(_allocationMap, 0, allocMapSize);
}

Memory::~Memory()
{
    delete[] _heap;
    delete[] _allocationMap;
}


uint32_t Memory::getTotalSize() const
{
    return _heapSize;
}

uint32_t Memory::getFreeSize() const
{
    return _allocator.getFreeSize() * _allocationSize;
}

Allocation::Ptr Memory::allocate(uint32_t size)
{
    const uint32_t allocationUnits = byteCountToUnitCount(size);
    const uint32_t unitIndex = _allocator.allocate(allocationUnits);
    markAsAllocated(unitIndex, allocationUnits);

    const uint32_t byteOffset = unitIndex * _allocationSize;
    const uint32_t byteSize = allocationUnits * _allocationSize;

    MemoryAccess *memAccess = this;
    Allocation::Ptr alloc = std::make_unique<Allocation>(memAccess, FIRST_USABLE_ADDRESS + byteOffset);
    _allocLen[alloc.get()] = byteSize;
    return std::move(alloc);
}

MemoryView Memory::getView(uint32_t address) noexcept
{
    return MemoryView(this, address);
}

void Memory::markAsAllocated(uint32_t startUnit, uint32_t numUnits)
{
    for (int i = startUnit; i<startUnit + numUnits; i++) {
        const uint32_t byte = i / 8;
        const uint32_t bit = i % 8;

        _allocationMap[byte] |= (1 << bit);
    }
}

void Memory::markAsFree(uint32_t startUnit, uint32_t numUnits)
{
    for (int i = startUnit; i<startUnit + numUnits; i++) {
        const uint32_t byte = i / 8;
        const uint32_t bit = i % 8;

        _allocationMap[byte] &= ~(1 << bit);
    }
}

bool Memory::isUnitAllocated(uint32_t unitIndex) const
{
    const uint32_t byte = unitIndex / 8;
    const uint32_t bit = unitIndex % 8;

    return _allocationMap[byte] & (1 << bit);
}

uint32_t Memory::byteOffsetToUnit(uint32_t byteOffset) const
{
    return byteOffset / _allocationSize;
}

uint32_t Memory::byteCountToUnitCount(uint32_t byteCount) const
{
    const uint32_t remainder = byteCount % _allocationSize;
    uint32_t units = byteCount / _allocationSize;
    if (remainder != 0) {
        units += 1;
    }

    return units;

}


/* MemoryAccess */
void Memory::onDeallocation(Allocation *allocation)
{
    if (!_allocLen.count(allocation)) {
        Throw(InvalidFreeException, "unable to free allocation");
    }

    const uint32_t byteOffset = allocation->getAddress() - FIRST_USABLE_ADDRESS;
    const uint32_t startUnit = byteOffsetToUnit(byteOffset);

    const uint32_t allocatedBytes = _allocLen[allocation];
    const uint32_t numUnits = byteCountToUnitCount(allocatedBytes);

    _allocator.deallocate(startUnit, numUnits);
    markAsFree(startUnit, numUnits);
    _allocLen.erase(allocation);
}

const uint8_t* Memory::read(uint32_t address, uint32_t len)
{
    if (address < FIRST_USABLE_ADDRESS || address + len > FIRST_USABLE_ADDRESS + _heapSize) {
        Throw(InvalidAccessException, "cannot access address 0x%x", address);
    }

    const uint32_t byteOffset = address - FIRST_USABLE_ADDRESS;
    const uint32_t firstUnit = byteOffsetToUnit(byteOffset);
    const uint32_t numUnits = byteCountToUnitCount(len);

    for (int i=0; i<numUnits; i++) {
        if (!isUnitAllocated(firstUnit + i)) {
            Throw(InvalidAccessException, "cannot access address 0x%x", address);
        }
    }

    return _heap + byteOffset;
}

void Memory::write(const uint8_t *buffer, uint32_t address, uint32_t len)
{
    if (address < FIRST_USABLE_ADDRESS || address + len > FIRST_USABLE_ADDRESS + _heapSize) {
        Throw(InvalidAccessException, "cannot access address 0x%x", address);
    }

    const uint32_t byteOffset = address - FIRST_USABLE_ADDRESS;
    const uint32_t firstUnit = byteOffsetToUnit(byteOffset);
    const uint32_t numUnits = byteCountToUnitCount(len);

    for (int i=0; i<numUnits; i++) {
        if (!isUnitAllocated(firstUnit + i)) {
            Throw(InvalidAccessException, "cannot access address 0x%x", address);
        }
    }

    memcpy(_heap + byteOffset, buffer, len);
}


}
}
