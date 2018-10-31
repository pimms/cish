#include <gtest/gtest.h>

#include "ast/Type.h"

using namespace cish::ast;


TEST(TypeTest, sizeTest)
{
    ASSERT_EQ(0, TypeDecl(TypeDecl::VOID).getSize());
    ASSERT_EQ(1, TypeDecl(TypeDecl::BOOL).getSize());
    ASSERT_EQ(1, TypeDecl(TypeDecl::CHAR).getSize());
    ASSERT_EQ(2, TypeDecl(TypeDecl::SHORT).getSize());
    ASSERT_EQ(4, TypeDecl(TypeDecl::INT).getSize());
    ASSERT_EQ(8, TypeDecl(TypeDecl::LONG).getSize());
    ASSERT_EQ(4, TypeDecl(TypeDecl::FLOAT).getSize());
    ASSERT_EQ(8, TypeDecl(TypeDecl::DOUBLE).getSize());
}

