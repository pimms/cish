#include "Allocator.h"


namespace cish::vm::internal
{

typedef std::list<Allocator::Block>::iterator BlockIterator;

/**
 * Return an iterator to the first block that has a HIGHER offset than 'offset'.
 */
BlockIterator lowerBoundOffset(BlockIterator first, BlockIterator last, const uint32_t& offset)
{
    // Adjusted implementation of listing found here:
    // http://www.cplusplus.com/reference/algorithm/lower_bound/<Paste>
    BlockIterator it;
    std::iterator_traits<BlockIterator>::difference_type count, step;
    count = distance(first,last);
    while (count > 0) {
        it = first;
        step=count/2;
        advance(it, step);

        if (it->offset < offset) {
            first = ++it;
            count -= step+1;
        } else {
            count = step;
        }
    }
    return first;
}

}

namespace cish::vm
{

Allocator::Allocator(uint32_t size):
    _size(size)
{
    Block block = { 0, _size };
    _blocks.push_back(block);
}

uint32_t Allocator::allocate(uint32_t size)
{
	for (auto it = _blocks.begin(); it != _blocks.end(); it++) {
        if (it->length >= size) {
            uint32_t addr = it->offset;
            it->offset += size;
            it->length -= size;

            if (it->length == 0) {
                _blocks.erase(it);
            }

            return addr;
        }
	}

    Throw(AllocationFailedException, "Failed to allocate %d units", size);
}

void Allocator::deallocate(uint32_t offset, uint32_t size)
{
    // lowerBoundOffset returns the position that would come *AFTER* a block at 'offset'.
    auto upper = internal::lowerBoundOffset(_blocks.begin(), _blocks.end(), offset);
    auto lower = upper;
    lower--;

    if (lower != _blocks.end() && lower->offset + lower->length == offset) {
        lower->length += size;

        if (upper != _blocks.end() && lower->offset + lower->length == upper->offset) {
            lower->length += upper->length;
            _blocks.erase(upper);
        }
    } else if (upper != _blocks.end() && offset + size == upper->offset) {
        upper->offset -= size;
        upper->length += size;
    } else {
        Block block = { offset, size };
        _blocks.insert(upper, block);
    }
}

uint32_t Allocator::getFreeSize() const
{
    uint32_t sum = 0;
    for (const auto &block: _blocks) {
        sum += block.length;
    }

    return sum;
}

const std::list<Allocator::Block>& Allocator::getBlocksByOffset() const
{
    return _blocks;
}


}
