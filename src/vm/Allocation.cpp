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

const uint8_t* MemoryView::readBuf(uint32_t length, uint32_t offset) const
{
    return _memoryAccess->read(_addr + offset, length);
}

ast::ExpressionValue MemoryView::evaluateAs(const ast::TypeDecl &type) const
{
    switch (type.getType()) {
        case ast::TypeDecl::BOOL:
            return ast::ExpressionValue(type, read<bool>());
        case ast::TypeDecl::CHAR:
            return ast::ExpressionValue(type, read<int8_t>());
        case ast::TypeDecl::SHORT:
            return ast::ExpressionValue(type, read<int16_t>());
        case ast::TypeDecl::INT:
            return ast::ExpressionValue(type, read<int32_t>());
        case ast::TypeDecl::FLOAT:
            return ast::ExpressionValue(type, read<float>());
        case ast::TypeDecl::DOUBLE:
            return ast::ExpressionValue(type, read<double>());
        case ast::TypeDecl::POINTER:
            return ast::ExpressionValue(type, read<uint32_t>());

        default:
            Throw(Exception, "Unhandled type: %s", type.getName());
    }
}

void MemoryView::writeBuf(const void *buffer, uint32_t len, uint32_t offset)
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
