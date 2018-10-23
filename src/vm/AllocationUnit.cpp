#include "AllocationUnit.h"

namespace cish
{
namespace vm
{

AllocationUnit::Ptr AllocationUnit::create(uint32_t offset, uint32_t len, MemoryAccess *memAccess)
{
    return AllocationUnit::Ptr(new AllocationUnit(offset, len, memAccess, true));
}

AllocationUnit::AllocationUnit(uint32_t offset, uint32_t len, MemoryAccess *memAccess, bool owner):
    _offset(offset),
    _length(len),
    _memoryAccess(memAccess),
    _owner(owner)
{

}

AllocationUnit::~AllocationUnit()
{
    _memoryAccess->onDeallocation(this);
}

uint32_t AllocationUnit::getOffset() const
{
    return _offset;
}

uint32_t AllocationUnit::getSize() const
{
    return _length;
}

AllocationUnit::Ptr AllocationUnit::read(uint32_t offset, uint32_t len) const
{
    if (offset + len > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    return AllocationUnit::Ptr(new AllocationUnit(_offset + offset, len, nullptr, false));
}


}
}
