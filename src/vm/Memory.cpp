#include "Memory.h"

#include <cassert>


namespace cish
{
namespace vm
{

Memory::Memory(uint32_t heapSize, uint32_t minAllocSize):
    _heapSize(heapSize),
    _allocationSize(minAllocSize)
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


AllocationUnit::Ptr Memory::allocate(uint32_t size)
{
    const uint32_t allocationUnits = bytesToAllocationUnits(size);
    const uint32_t unitIndex = findUnallocatedRun(allocationUnits);

    markAsAllocated(unitIndex, allocationUnits);

    const uint32_t byteOffset = unitIndex * _allocationSize;
    const uint32_t byteSize = allocationUnits * _allocationSize;

    return AllocationUnit::create(byteOffset, byteSize, this);
}


uint32_t Memory::findUnallocatedRun(uint32_t requiredUnits)
{
    // TODO: This can be optimized a boatload - there's no need to run through
    // the whole memory space, lol

    const uint32_t numUnits = _heapSize / _allocationSize;

    uint32_t cur = 0;
    uint32_t runLenght = 0;

    // Find the first unoccupied run of 'length' bytes in the memory space.
    for (int i=0; i<numUnits; i++) {
        const uint32_t byte = i / 8;
        const uint32_t bit = i % 8;

        if ((_allocationMap[byte] & (1 << bit)) == 0) {
            if (!runLenght) {
                cur = i;
            }

            runLenght++;

            if (runLenght >= requiredUnits) {
                return cur;
            }
        } else {
            runLenght = 0;
        }
    }

    throw OutOfMemoryException("Allocation failed");
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

uint32_t Memory::bytesToAllocationUnits(uint32_t byteCount) const
{
    const uint32_t remainder = byteCount % _allocationSize;
    uint32_t units = byteCount / _allocationSize;
    if (remainder != 0) {
        units += 1;
    }

    return units;

}


/* MemoryAccess */
uint8_t* Memory::getHeap()
{
    return _heap;
}

void Memory::onDeallocation(AllocationUnit *allocation)
{
    const uint32_t startUnit = allocation->getOffset() / _allocationSize;
    const uint32_t numUnits = bytesToAllocationUnits(allocation->getSize());

    markAsFree(startUnit, numUnits);
}


}
}
