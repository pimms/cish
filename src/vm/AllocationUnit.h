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
class DeallocationDelegate
{
public:
    virtual void onDeallocation(AllocationUnit *unit) = 0;
};


class AllocationUnit
{
public:
    typedef std::unique_ptr<AllocationUnit> Ptr;

    static Ptr create(uint8_t const *ptr, uint32_t len, DeallocationDelegate *delegate);
    AllocationUnit(uint8_t const *ptr, uint32_t len, DeallocationDelegate *delegate);
    ~AllocationUnit();

    uint32_t getSize() const;
    const uint8_t * const getPointer() const;

    AllocationUnit::Ptr read(uint32_t offset, uint32_t len) const;

    template<typename T>
    T read() const;

    template<typename T>
    void write(T value, uint32_t offset = 0);

private:
    uint8_t const *_ptr;
    const uint32_t _length;
    DeallocationDelegate *_delegate;
};


template<typename T>
T AllocationUnit::read() const
{
    const size_t size = sizeof(T);
    if (size > _length) {
        throw InvalidAccessException("Read access out of bounds");
    }

    T* castedPtr = (T*)_ptr;
    return *castedPtr;
}

template<typename T>
void AllocationUnit::write(T value, uint32_t offset)
{
    const size_t size = sizeof(T);
    if (size > offset + _length) {
        throw InvalidAccessException("Write access out of bounds");
    }

    T* castedPtr = (T*)(_ptr + offset);
    *castedPtr = value;
}



}
}

