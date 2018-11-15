#include <gtest/gtest.h>

#include "vm/Variable.h"
#include "vm/Memory.h"


using namespace cish::vm;
using namespace cish::ast;

TEST(VariableTest, tooSmallAllocationDoesNotThrow)
{
    Memory memory(100, 1);
    ASSERT_NO_THROW(Variable(TypeDecl::INT, memory.allocate(1)));
    ASSERT_NO_THROW(Variable(TypeDecl::INT, memory.allocate(2)));
    ASSERT_NO_THROW(Variable(TypeDecl::INT, memory.allocate(3)));
}

TEST(VariableTest, getAllocationDoesNotThrow)
{
    Memory memory(100, 1);

    ASSERT_NO_THROW(Variable(TypeDecl::INT, std::move(memory.allocate(4))));
    ASSERT_NO_THROW(Variable(TypeDecl::INT, std::move(memory.allocate(5))));
    ASSERT_NO_THROW(Variable(TypeDecl::INT, std::move(memory.allocate(50))));
}

TEST(VariableTest, heapOffsetIsInherited)
{
    Memory memory(100, 1);

    Allocation::Ptr alloc1 = memory.allocate(4);
    Allocation::Ptr alloc2 = memory.allocate(4);

    const uint32_t offset1 = alloc1->getAddress();
    const uint32_t offset2 = alloc2->getAddress();

    auto var1 = Variable(TypeDecl::INT, std::move(alloc1));
    auto var2 = Variable(TypeDecl::INT, std::move(alloc2));

    ASSERT_EQ(offset1, var1.getHeapAddress());
    ASSERT_EQ(offset2, var2.getHeapAddress());
}



