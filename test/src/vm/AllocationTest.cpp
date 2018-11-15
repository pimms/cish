#include <gtest/gtest.h>

#include "vm/Allocation.h"
#include "vm/Memory.h"


using namespace cish::vm;


TEST(AllocationTest, WriteOutOfBoundsThrows)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    ASSERT_THROW(alloc->write<uint32_t>(25, 1), InvalidAccessException);
}

TEST(AllocationTest, ReadOutOfBoundsThrows)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    ASSERT_THROW(alloc->read<uint32_t>(1), InvalidAccessException);
}

TEST(AllocationTest, WriteAndReadDword)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    alloc->write<uint32_t>(32);
    ASSERT_EQ(32, alloc->read<uint32_t>());

    alloc->write<uint32_t>(34);
    ASSERT_EQ(34, alloc->read<uint32_t>());
}

TEST(AllocationTest, AllocationsAreLittleEndian)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    alloc->write<uint32_t>(255);
    ASSERT_EQ(255, alloc->read<uint8_t>());
}

TEST(AllocationTest, WriteAtOffsetWorks)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    alloc->write<uint8_t>(1, 0);
    alloc->write<uint8_t>(2, 1);
    alloc->write<uint8_t>(3, 2);
    alloc->write<uint8_t>(4, 3);

    for (int i=0; i<4; i++)
        ASSERT_EQ(i+1, alloc->read<uint8_t>(i));

    const uint32_t val = alloc->read<uint32_t>();
    ASSERT_EQ(1 | 2 << 8 | 3 << 16 | 4 << 24, val);
}

TEST(AllocationTest, ReadAtOffsetWorks)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    alloc->write<uint32_t>(1 | 2 << 8 | 3 << 16 | 4 << 24);

    for (int i=0; i<4; i++)
        ASSERT_EQ(i+1, alloc->read<uint8_t>(i));
}

