#include <gtest/gtest.h>

#include "ast/StructLayout.h"

using namespace cish::ast;


TEST(StrutLayoutTest, emptyStructsThrows)
{
    ASSERT_THROW(StructLayout layout("empty_lol", {}), EmptyStructException);
}

TEST(StructLayoutTest, fieldNamesMustBeUnique)
{
    ASSERT_THROW(
            StructLayout l("s", {
                {TypeDecl::INT, "foo"}, 
                {TypeDecl::FLOAT, "foo"}
            }),
            FieldAlreadyDeclaredException);
}

TEST(StructLayoutTest, simpleValidStruct)
{
    ASSERT_NO_THROW(
            StructLayout l("s", {
                {TypeDecl::INT, "my_int"}, 
                {TypeDecl::FLOAT, "my_float"}
            }));
}

TEST(StructLayoutTest, offset)
{
    StructLayout layout("s", {
        {TypeDecl::INT, "my_int"},
        {TypeDecl::BOOL, "my_bool"},
        {TypeDecl::FLOAT, "my_float"},
        {TypeDecl::getPointer(TypeDecl::VOID), "my_ptr"},
    });

    ASSERT_EQ(0, layout.getField("my_int")->getOffset());
    ASSERT_EQ(4, layout.getField("my_bool")->getOffset());
    ASSERT_EQ(5, layout.getField("my_float")->getOffset());
    ASSERT_EQ(9, layout.getField("my_ptr")->getOffset());
}

TEST(StructLayoutTest, size)
{
    StructLayout layout("s", {
        {TypeDecl::INT, "my_int"},
        {TypeDecl::BOOL, "my_bool"},
        {TypeDecl::FLOAT, "my_float"},
        {TypeDecl::getPointer(TypeDecl::VOID), "my_ptr"},
    });

    ASSERT_EQ(13, layout.getSize());
}

TEST(StructLayoutTest, invalidFieldLookupThrows)
{
    StructLayout layout("s", {{TypeDecl::INT, "foo"}});
    ASSERT_THROW(layout.getField("lol"), NoSuchFieldException);
}

