#include "Allocation.h"

namespace cish
{
namespace vm
{

Allocation::Ptr Allocation::create(uint32_t offset, uint32_t len, MemoryAccess *memAccess)
{
    return Allocation::Ptr(new Allocation(offset, len, memAccess, true));
}

Allocation::Allocation(uint32_t offset, uint32_t len, MemoryAccess *memAccess, bool owner):
    _offset(offset),
    _length(len),
    _memoryAccess(memAccess),
    _owner(owner)
{
    assert(len > 0);
    assert(_memoryAccess != nullptr);
}

Allocation::~Allocation()
{
    if (_owner) {
        _memoryAccess->onDeallocation(this);
    }
}

uint32_t Allocation::getOffset() const
{
    return _offset;
}

uint32_t Allocation::getSize() const
{
    return _length;
}

Allocation::Ptr Allocation::read(uint32_t offset, uint32_t len) const
{
    if (offset + len > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    return Allocation::Ptr(new Allocation(_offset + offset, len, _memoryAccess, false));
}


}
}
