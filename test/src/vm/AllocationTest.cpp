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

TEST(AllocationTest, SubviewsHaveOffsetsRelativeToParent)
{
    Memory memory(12, 4);
    auto alloc1 = memory.allocate(4);
    auto alloc2 = memory.allocate(4);
    auto alloc3 = memory.allocate(4);

    auto sub1 = alloc1->read(2, 2);
    auto sub2 = alloc2->read(1, 3);
    auto sub3 = alloc3->read(0, 4);

    ASSERT_EQ(alloc1->getOffset() + 2, sub1->getOffset());
    ASSERT_EQ(alloc2->getOffset() + 1, sub2->getOffset());
    ASSERT_EQ(alloc3->getOffset() + 0, sub3->getOffset());

    ASSERT_EQ(2, sub1->getSize());
    ASSERT_EQ(3, sub2->getSize());
    ASSERT_EQ(4, sub3->getSize());
}

TEST(AllocationTest, OutOfBoundsSubviewsThrows)
{
    Memory memory(4, 4);
    auto alloc = memory.allocate(4);

    ASSERT_THROW(alloc->read(0, 8), InvalidAccessException);
    ASSERT_THROW(alloc->read(1, 4), InvalidAccessException);
    ASSERT_THROW(alloc->read(5, 1), InvalidAccessException);
}

TEST(AllocationTest, SubviewMapsToParentMemory)
{
    Memory memory(4, 4);
    auto parent = memory.allocate(4);

    parent->write<uint32_t>(0x44332211);

    // Create a subview into bytes 2 & 3
    auto child1 = parent->read(1, 2);
    // Create a subview into bytes 3 & 4
    auto child2 = parent->read(2, 2);
    // Create a sub-subview into byte 4
    auto child3 = child2->read(1, 1);

    ASSERT_EQ(0x3322, child1->read<uint16_t>());
    ASSERT_EQ(0x4433, child2->read<uint16_t>());
    ASSERT_EQ(0x44, child3->read<uint8_t>());

    parent->write<uint32_t>(0xFFEEDDCC);
    ASSERT_EQ(0xEEDD, child1->read<uint16_t>());
    ASSERT_EQ(0xFFEE, child2->read<uint16_t>());
    ASSERT_EQ(0xFF, child3->read<uint8_t>());
}

