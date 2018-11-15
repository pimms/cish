#include <gtest/gtest.h>

#include "vm/Memory.h"
#include "vm/Allocation.h"

using namespace cish::vm;



void ensureReadableMemory(Memory *mem, uint32_t address, uint32_t size)
{
    for (int i=0; i<size; i++) {
        MemoryView view = mem->getView(address);
        ASSERT_NO_THROW(view.read<uint8_t>(i));
    }
}


TEST(MemoryTest, AllocationMatchesMinAllocSize)
{
    const uint32_t minAllocSize = 5;

    Memory memory(8, minAllocSize);
    auto allocation = memory.allocate(2);

    ensureReadableMemory(&memory, allocation->getAddress(), 4);
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

    Allocation::Ptr alloc;

    alloc = memory.allocate(1);
    ensureReadableMemory(&memory, alloc->getAddress(), 4);

    alloc = memory.allocate(2);
    ensureReadableMemory(&memory, alloc->getAddress(), 4);

    alloc = memory.allocate(3);
    ensureReadableMemory(&memory, alloc->getAddress(), 4);

    alloc = memory.allocate(4);
    ensureReadableMemory(&memory, alloc->getAddress(), 4);


    alloc = memory.allocate(5);
    ensureReadableMemory(&memory, alloc->getAddress(), 8);

    alloc = memory.allocate(6);
    ensureReadableMemory(&memory, alloc->getAddress(), 8);

    alloc = memory.allocate(7);
    ensureReadableMemory(&memory, alloc->getAddress(), 8);

    alloc = memory.allocate(8);
    ensureReadableMemory(&memory, alloc->getAddress(), 8);
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
    // Of the total 5, only 4 are available because of misalignment
    Memory memory(5, 4);
    ASSERT_EQ(4, memory.getFreeSize());

    auto alloc = memory.allocate(4);
    ASSERT_EQ(0, memory.getFreeSize());

    ASSERT_THROW(memory.allocate(4), OutOfMemoryException);
}

TEST(MemoryTest, DeallocatingMakesMemoryAvailableAgain)
{
    Memory memory(8, 4);
    ASSERT_EQ(8, memory.getFreeSize());

    auto alloc1 = memory.allocate(4);
    ASSERT_EQ(4, memory.getFreeSize());

    auto alloc2 = memory.allocate(4);
    ASSERT_EQ(0, memory.getFreeSize());

    ASSERT_THROW(memory.allocate(4), OutOfMemoryException);
    alloc1 = nullptr;
    ASSERT_EQ(4, memory.getFreeSize());

    ASSERT_NO_THROW(memory.allocate(4));
}


TEST(MemoryTest, WritesDoesNotAffectOtherVariables)
{
    Memory memory(100, 1);

    auto alloc1 = memory.allocate(1);
    auto alloc2 = memory.allocate(2);
    auto alloc3 = memory.allocate(4);
    auto alloc4 = memory.allocate(8);

    alloc1->write<char>(1);
    alloc2->write<short>(2);
    alloc3->write<int>(3);
    alloc4->write<long>(4);

    ASSERT_EQ(1, alloc1->read<char>());
    ASSERT_EQ(2, alloc2->read<short>());
    ASSERT_EQ(3, alloc3->read<int>());
    ASSERT_EQ(4, alloc4->read<long>());
}


TEST(MemoryTest, viewOverlappingAllocationIsFine)
{
    Memory memory(100, 1);

    auto alloc = memory.allocate(4);
    auto view = memory.getView(alloc->getAddress());

    alloc->write<uint32_t>(15);

    ASSERT_EQ(15, view.read<uint32_t>());
    ASSERT_NO_THROW(view.write<uint32_t>(16));

    ASSERT_EQ(16, alloc->read<uint32_t>());
}

TEST(MemoryTest, deallocatingVariableMakesViewsThrow)
{
    Memory memory(100, 1);

    auto alloc = memory.allocate(4);
    auto view = memory.getView(alloc->getAddress());
    ASSERT_NO_THROW(view.read<uint32_t>());

    alloc = nullptr;

    ASSERT_THROW(view.read<uint32_t>(), InvalidAccessException);
}

TEST(MemoryTest, readingOutOfBoundsThrows)
{
    Memory memory(100, 1);

    auto alloc = memory.allocate(4);

    // Explicitly read with offset 1
    auto alignedView = memory.getView(alloc->getAddress());
    ASSERT_THROW(alignedView.read<uint32_t>(1), InvalidAccessException);

    // Implicitly read with offset 1
    auto offsetView = memory.getView(alloc->getAddress() + 1);
    ASSERT_THROW(offsetView.read<uint32_t>(), InvalidAccessException);
}

TEST(MemoryTest, writingOutOfBoundsThrows)
{
    Memory memory(100, 1);

    auto alloc = memory.allocate(4);

    // Explicitly write with offset 1
    auto alignedView = memory.getView(alloc->getAddress());
    ASSERT_THROW(alignedView.write<uint32_t>(1, 1), InvalidAccessException);

    // Implicitly write with offset 1
    auto offsetView = memory.getView(alloc->getAddress() + 1);
    ASSERT_THROW(offsetView.write<uint32_t>(1), InvalidAccessException);
}

TEST(MemoryTest, viewsAreRetroactivelyValid)
{
    Memory memory(100, 1);

    MemoryView view = memory.getView(Memory::firstUsableMemoryAddress());

    // We're invalid at this point
    ASSERT_THROW(view.read<uint32_t>(), InvalidAccessException);

    auto alloc = memory.allocate(4);
    alloc->write<uint32_t>(15);
    ASSERT_EQ(alloc->getAddress(), view.getAddress());

    // This is fine - a variable has been allocated in its' place
    ASSERT_EQ(15, view.read<uint32_t>());
}

TEST(MemoryTest, nullViewsAreOkUntilAccessed)
{
    Memory memory(100, 1);

    MemoryView view = memory.getView(0);
    ASSERT_THROW(view.read<uint32_t>(), InvalidAccessException);
    ASSERT_THROW(view.write<uint32_t>(1), InvalidAccessException);
}
