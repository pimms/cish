#include <gtest/gtest.h>

#include "vm/Memory.h"

using namespace cish::vm;


TEST(MemoryTest, AllocationMatchesMinAllocSize)
{
    const uint32_t minAllocSize = 5;

    Memory memory(8, minAllocSize);
    auto allocation = memory.allocate(2);

    ASSERT_EQ(minAllocSize, allocation->getSize());
}

TEST(MemoryTest, AbleToAllocateAllMemory)
{
    // The key takeaway from this test is that
    // we don't crash I guess lol
    Memory memory(8, 4);
    auto alloc1 = memory.allocate(4);
    auto alloc2 = memory.allocate(4);
}

TEST(MemoryTest, UnalignedAllocationsAreAligned)
{
    Memory memory(128, 4);

    ASSERT_EQ(4, memory.allocate(1)->getSize());
    ASSERT_EQ(4, memory.allocate(2)->getSize());
    ASSERT_EQ(4, memory.allocate(3)->getSize());
    ASSERT_EQ(4, memory.allocate(4)->getSize());

    ASSERT_EQ(8, memory.allocate(5)->getSize());
    ASSERT_EQ(8, memory.allocate(6)->getSize());
    ASSERT_EQ(8, memory.allocate(7)->getSize());
    ASSERT_EQ(8, memory.allocate(8)->getSize());
}

TEST(MemoryTest, AllocationFailureThrows)
{
    Memory memory(8, 4);

    auto alloc1 = memory.allocate(4);
    auto alloc2 = memory.allocate(4);

    ASSERT_THROW(memory.allocate(4), OutOfMemoryException);
}

TEST(MemoryTest, TooLargeAllocationThrows)
{
    Memory memory(100, 4);
    ASSERT_THROW(memory.allocate(101), OutOfMemoryException);
}

TEST(MemoryTest, UnalignedHeapSize)
{
    Memory memory(5, 4);
    auto alloc = memory.allocate(4);
    ASSERT_THROW(memory.allocate(4), OutOfMemoryException);
}

TEST(MemoryTest, DeallocatingMakesMemoryAvailableAgain)
{
    Memory memory(8, 4);
    auto alloc1 = memory.allocate(4);
    auto alloc2 = memory.allocate(4);

    ASSERT_THROW(memory.allocate(4), OutOfMemoryException);
    alloc1 = nullptr;
    ASSERT_NO_THROW(memory.allocate(4));
}

