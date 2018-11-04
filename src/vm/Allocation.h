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


class Allocation;
class MemoryAccess
{
public:
    virtual ~MemoryAccess() = default;
    virtual uint8_t* getHeap() = 0;
    virtual void onDeallocation(Allocation *allocation) = 0;
};


class Allocation
{
public:
    typedef std::unique_ptr<Allocation> Ptr;

    static Ptr create(uint32_t offset, uint32_t len, MemoryAccess *memAccess);
    ~Allocation();

    uint32_t getOffset() const;
    uint32_t getSize() const;

    Allocation::Ptr read(uint32_t offset, uint32_t len) const;

    template<typename T>
    T read(uint32_t offset = 0) const;

    template<typename T>
    void write(T value, uint32_t offset = 0);

private:
    const uint32_t _offset;
    const uint32_t _length;
    const bool _owner;
    MemoryAccess *_memoryAccess;

    Allocation(uint32_t offset, uint32_t len, MemoryAccess *memAccess, bool owner);
};


template<typename T>
T Allocation::read(uint32_t offset) const
{
    const size_t size = sizeof(T);
    if (size + offset > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    T* castedPtr = (T*)(_memoryAccess->getHeap() + offset + _offset);
    return *castedPtr;
}

template<typename T>
void Allocation::write(T value, uint32_t offset)
{
    const size_t size = sizeof(T);
    if (size + offset > _length) {
        throw InvalidAccessException("Write access out of bounds");
    }

    T* castedPtr = (T*)(_memoryAccess->getHeap() + offset + _offset);
    *castedPtr = value;
}


}
}

