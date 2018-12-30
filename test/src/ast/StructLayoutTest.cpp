#include <gtest/gtest.h>

#include "ast/StructLayout.h"

using namespace cish::ast;


/* TODO: How should we check this now? Finalization?
TEST(StrutLayoutTest, emptyStructsThrows)
{
    ASSERT_THROW(StructLayout layout("empty_lol", {}), EmptyStructException);
}
*/

TEST(StructLayoutTest, fieldNamesMustBeUnique)
{
    StructLayout l("s");
    l.addField(TypeDecl::INT, "foo");
    ASSERT_THROW(l.addField(TypeDecl::FLOAT, "foo"),
                 FieldAlreadyDeclaredException);
}

TEST(StructLayoutTest, simpleValidStruct)
{
    StructLayout l("s");
    ASSERT_NO_THROW(l.addField(TypeDecl::INT, "my_int"));
    ASSERT_NO_THROW(l.addField(TypeDecl::FLOAT, "my_float"));
}

TEST(StructLayoutTest, offset)
{
    StructLayout layout("s");
    layout.addField(TypeDecl::INT, "my_int");
    layout.addField(TypeDecl::BOOL, "my_bool");
    layout.addField(TypeDecl::FLOAT, "my_float");
    layout.addField(TypeDecl::getPointer(TypeDecl::VOID), "my_ptr");

    ASSERT_EQ(0, layout.getField("my_int")->getOffset());
    ASSERT_EQ(4, layout.getField("my_bool")->getOffset());
    ASSERT_EQ(5, layout.getField("my_float")->getOffset());
    ASSERT_EQ(9, layout.getField("my_ptr")->getOffset());
}

TEST(StructLayoutTest, size)
{
    StructLayout layout("s");
    layout.addField(TypeDecl::INT, "my_int");
    layout.addField(TypeDecl::BOOL, "my_bool");
    layout.addField(TypeDecl::FLOAT, "my_float");
    layout.addField(TypeDecl::getPointer(TypeDecl::VOID), "my_ptr");

    ASSERT_EQ(13, layout.getSize());
}

TEST(StructLayoutTest, invalidFieldLookupThrows)
{
    StructLayout layout("s"); 
    layout.addField(TypeDecl::INT, "foo");
    ASSERT_THROW(layout.getField("lol"), NoSuchFieldException);
}

