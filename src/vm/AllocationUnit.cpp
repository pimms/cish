#include "AllocationUnit.h"

namespace cish
{
namespace vm
{

AllocationUnit::Ptr AllocationUnit::create(uint8_t const *ptr, uint32_t len, DeallocationDelegate *delegate)
{
    return AllocationUnit::Ptr(new AllocationUnit(ptr, len, delegate));
}

AllocationUnit::AllocationUnit(uint8_t const *ptr, uint32_t len, DeallocationDelegate *delegate):
    _ptr(ptr),
    _length(len),
    _delegate(delegate)
{

}

AllocationUnit::~AllocationUnit()
{
    if (_delegate) {
        _delegate->onDeallocation(this);
    }
}

uint32_t AllocationUnit::getSize() const
{
    return _length;
}


const uint8_t * const AllocationUnit::getPointer() const
{
    return _ptr;
}

AllocationUnit::Ptr AllocationUnit::read(uint32_t offset, uint32_t len) const
{
    if (offset + len > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    // Do NOT pass our delegate, as this subview has no
    // business deallocating our memory
    return AllocationUnit::Ptr(new AllocationUnit(_ptr + offset, len, nullptr));
}


}
}
