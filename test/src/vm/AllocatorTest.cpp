#include <gtest/gtest.h>

#include "vm/Allocator.h"

using namespace cish::vm;


void printBlockLists(std::list<Allocator::Block> &actual, std::vector<Allocator::Block> &expected)
{
    printf("  Actual:   { ");
    for (auto block: actual) {
        printf("[%d, %d] ", block.offset, block.length);
    }
    printf("}\n");

    printf("  Expected: { ");
    for (auto block: expected) {
        printf("[%d, %d] ", block.offset, block.length);
    }
    printf("}\n");
}

void assertBlockStructure(const Allocator &allocator, std::vector<Allocator::Block> expected)
{
    auto actual = allocator.getBlocksByOffset();

    if (expected.size() != actual.size()) {
        printf("Invalid number of blocks\n");
        printBlockLists(actual, expected);
        FAIL();
    }

    int i = 0;
    for (auto block: actual) {
        if (expected[i].offset != block.offset || expected[i].length != block.length) {
            printf("Unequal block at index %d\n", i);
            printBlockLists(actual, expected);
            FAIL();
        }

        i++;
    }
}


TEST(AllocatorTest, basicAllocation)
{
    Allocator allocator(100);
    ASSERT_EQ(0, allocator.allocate(4));
}

TEST(AllocatorTest, consecutiveAllocations)
{
    Allocator allocator(100);
    ASSERT_EQ(0, allocator.allocate(4));
    ASSERT_EQ(4, allocator.allocate(2));
    ASSERT_EQ(6, allocator.allocate(1));
    ASSERT_EQ(7, allocator.allocate(3));
    ASSERT_EQ(10, allocator.allocate(10));
    ASSERT_EQ(20, allocator.allocate(10));
}

TEST(AllocatorTest, allocatingTooMuchThrows)
{
    Allocator allocator(5);
    ASSERT_EQ(0, allocator.allocate(1));
    ASSERT_EQ(1, allocator.allocate(1));
    ASSERT_EQ(2, allocator.allocate(1));
    ASSERT_EQ(3, allocator.allocate(1));
    ASSERT_EQ(4, allocator.allocate(1));
    ASSERT_THROW(allocator.allocate(1), cish::vm::AllocationFailedException);
}


TEST(AllocatorTest, deallocationFreesUnits)
{
    Allocator allocator(10);
    assertBlockStructure(allocator, {{0, 10}});

    ASSERT_EQ(0, allocator.allocate(10));
    assertBlockStructure(allocator, {});

    allocator.deallocate(0, 10);
    assertBlockStructure(allocator, {{0, 10}});

    ASSERT_EQ(0, allocator.allocate(10));
    assertBlockStructure(allocator, {});
}

TEST(AllocatorTest, deallocWhenNoUpperNeighbour)
{
    // Verify that we are able to expand if we ONLY have a lower neighbour 
    Allocator alloc(20);
    alloc.allocate(20);
    alloc.deallocate(5, 10);
    assertBlockStructure(alloc, {{5, 10}});

    alloc.deallocate(15, 5);
    assertBlockStructure(alloc, {{5, 15}});
}

TEST(AllocatorTest, deallocWhenNoLowerNeighbour)
{
    // Verify that we are able to expand if we ONLY have a lower neighbour 
    Allocator alloc(20);
    alloc.allocate(20);
    alloc.deallocate(5, 10);
    assertBlockStructure(alloc, {{5, 10}});

    alloc.deallocate(0, 5);
    assertBlockStructure(alloc, {{0, 15}});
}

TEST(AllocatorTest, semiAdvancedFragmenting)
{
    Allocator alloc(20);

    ASSERT_EQ(0, alloc.allocate(4));
    assertBlockStructure(alloc, {{4, 16}});
    ASSERT_EQ(4, alloc.allocate(4));
    assertBlockStructure(alloc, {{8, 12}});
    ASSERT_EQ(8, alloc.allocate(4));
    assertBlockStructure(alloc, {{12, 8}});

    alloc.deallocate(4, 4);
    assertBlockStructure(alloc, {{4, 4}, {12, 8}});

    alloc.deallocate(0, 2);
    assertBlockStructure(alloc, {{0, 2}, {4, 4}, {12, 8}});

    ASSERT_EQ(4, alloc.allocate(3));
    assertBlockStructure(alloc, {{0, 2}, {7, 1}, {12, 8}});

    alloc.deallocate(5, 2);
    assertBlockStructure(alloc, {{0, 2}, {5, 3}, {12, 8}});

    alloc.deallocate(2, 2);
    assertBlockStructure(alloc, {{0, 4}, {5, 3}, {12, 8}});

    ASSERT_EQ(12, alloc.allocate(6));
    assertBlockStructure(alloc, {{0, 4}, {5, 3}, {18, 2}});

    ASSERT_THROW(alloc.allocate(9), AllocationFailedException);
    assertBlockStructure(alloc, {{0, 4}, {5, 3}, {18, 2}});

    alloc.deallocate(8, 10);
    assertBlockStructure(alloc, {{0, 4}, {5, 15}});

    alloc.deallocate(4, 1);
    assertBlockStructure(alloc, {{0, 20}});
}

