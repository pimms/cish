#pragma once

#include <memory>
#include <stdexcept>

namespace cish
{
namespace vm
{


/*
==================
MemoryAccess
==================
*/
class Allocation;
class MemoryAccess
{
public:
    virtual ~MemoryAccess() = default;

    virtual void onDeallocation(Allocation *allocation) = 0;
    virtual const uint8_t* read(uint32_t address, uint32_t len) = 0;
    virtual void write(const uint8_t *buffer, uint32_t address, uint32_t len) = 0;
};

/*
==================
MemoryView

A dumb view into memory with no concept of ownership.
==================
*/
class MemoryView
{
public:
    MemoryView(MemoryAccess *memAccess, uint32_t address);
    virtual ~MemoryView() = default;

    uint32_t getAddress() const;

    template<typename T>
    T read(uint32_t offset = 0) const;

    template<typename T>
    void write(T value, uint32_t offset = 0);

    void writeBuf(const void *buffer, uint32_t len, uint32_t offset = 0);

protected:
    MemoryAccess *_memoryAccess;

private:
    uint32_t _addr;
};

template<typename T>
T MemoryView::read(uint32_t offset) const
{
    const size_t tsize = sizeof(T);
    const uint8_t *rawPtr = _memoryAccess->read(_addr + offset, tsize);
    const T* castedPtr = (const T*)(rawPtr);
    return *castedPtr;
}

template<typename T>
void MemoryView::write(T value, uint32_t offset)
{
    const size_t size = sizeof(T);
    const uint8_t *rawPtr = (const uint8_t*)&value;
    _memoryAccess->write(rawPtr, _addr + offset, sizeof(T));
}



/*
==================
Allocation

An allocated (i.e., owned & safe) chunk of memory.
==================
*/
class Allocation: public MemoryView
{
public:
    typedef std::unique_ptr<Allocation> Ptr;

    Allocation(MemoryAccess *memAccess, uint32_t addr);
    ~Allocation();
};


}
}

