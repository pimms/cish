#include "Allocation.h"

namespace cish
{
namespace vm
{



/*
==================
MemoryView
==================
*/
MemoryView::MemoryView(MemoryAccess *memAccess, uint32_t address):
    _memoryAccess(memAccess),
    _addr(address)
{
    assert(_memoryAccess != nullptr);
}

uint32_t MemoryView::getAddress() const
{
    return _addr;
}

void MemoryView::copy(const void *buffer, uint32_t len, uint32_t offset)
{
    _memoryAccess->write((const uint8_t*)buffer, _addr+offset, len);
}

/*
==================
Allocation
==================
*/
Allocation::Allocation(MemoryAccess *memAccess, uint32_t offset):
    MemoryView(memAccess, offset)
{
}

Allocation::~Allocation()
{
    _memoryAccess->onDeallocation(this);
}


}
}
