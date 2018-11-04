#include <gtest/gtest.h>

#include "ast/Type.h"
#include "ast/AstNodes.h"

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

TEST(TypeTest, resolveFromString)
{
    ASSERT_EQ(TypeDecl(TypeDecl::VOID), TypeDecl::getFromString("void"));
    ASSERT_EQ(TypeDecl(TypeDecl::BOOL), TypeDecl::getFromString("bool"));
    ASSERT_EQ(TypeDecl(TypeDecl::CHAR), TypeDecl::getFromString("char"));
    ASSERT_EQ(TypeDecl(TypeDecl::SHORT), TypeDecl::getFromString("short"));
    ASSERT_EQ(TypeDecl(TypeDecl::INT), TypeDecl::getFromString("int"));
    ASSERT_EQ(TypeDecl(TypeDecl::LONG), TypeDecl::getFromString("long"));
    ASSERT_EQ(TypeDecl(TypeDecl::FLOAT), TypeDecl::getFromString("float"));
    ASSERT_EQ(TypeDecl(TypeDecl::DOUBLE), TypeDecl::getFromString("double"));
}

TEST(TypeTest, resolveFromStringThrowsIfUnrecognized)
{
    // We might support these at some point, but not right now
    ASSERT_THROW(TypeDecl::getFromString("unsigned int"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("long int"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("unsigned long int"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("unsigned long long"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("long double"), InvalidTypeException);

    // These would probably fall under the "user-defined type" umbrella,
    // but we don't support those yet, so expect exceptions.
    ASSERT_THROW(TypeDecl::getFromString("my_struct"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("whatsthis"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("uint8_t"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("uint32_t"), InvalidTypeException);
    ASSERT_THROW(TypeDecl::getFromString("phartyphockborlz"), InvalidTypeException);
}

