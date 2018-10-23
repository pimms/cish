#pragma once

#include <memory>
#include <stdexcept>

namespace cish
{
namespace vm
{


class InvalidAccessException : public std::runtime_error
{
public:
    InvalidAccessException(const char *message): std::runtime_error(message) {}
};


class AllocationUnit;
class MemoryAccess
{
public:
    virtual uint8_t* getHeap() = 0;
    virtual void onDeallocation(AllocationUnit *allocation) = 0;
};


class AllocationUnit
{
public:
    typedef std::unique_ptr<AllocationUnit> Ptr;

    static Ptr create(uint32_t offset, uint32_t len, MemoryAccess *memAccess);
    ~AllocationUnit();

    uint32_t getOffset() const;
    uint32_t getSize() const;

    AllocationUnit::Ptr read(uint32_t offset, uint32_t len) const;

    template<typename T>
    T read() const;

    template<typename T>
    void write(T value, uint32_t offset = 0);

private:
    const uint32_t _offset;
    const uint32_t _length;
    const bool _owner;
    MemoryAccess *_memoryAccess;

    AllocationUnit(uint32_t offset, uint32_t len, MemoryAccess *memAccess, bool owner);
};


template<typename T>
T AllocationUnit::read() const
{
    const size_t size = sizeof(T);
    if (size > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    T* castedPtr = (T*)_memoryAccess->getHeap() + _offset;
    return *castedPtr;
}

template<typename T>
void AllocationUnit::write(T value, uint32_t offset)
{
    const size_t size = sizeof(T);
    if (size > offset + _length) {
        throw InvalidAccessException("Write access out of bounds");
    }

    T* castedPtr = (T*)(_memoryAccess->getHeap() + offset);
    *castedPtr = value;
}



}
}

