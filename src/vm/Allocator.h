#pragma once

#include <stdint.h>
#include <list>

#include "../Exception.h"

namespace cish
{
namespace vm
{

DECLARE_EXCEPTION(AllocationFailedException);
DECLARE_EXCEPTION(InvalidDeallocationException);

class Allocator
{
public:
    struct Block
    {
        uint32_t offset;
        uint32_t length;
    };

    Allocator(uint32_t size);

	uint32_t allocate(uint32_t size);
    void deallocate(uint32_t offset, uint32_t size);

    // !! Should only be used for test purposes !! //
    const std::list<Block>& getBlocksByOffset() const;

private:
    std::list<Block> _blocks;

    uint32_t _size;
};


}
}

