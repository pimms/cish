#include <gtest/gtest.h>

#include "HelloWorld.h"

TEST(DummyTest, ShouldPass)
{
    ASSERT_EQ(15, cish::foo());
}

