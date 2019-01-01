#include <gtest/gtest.h>

#include "ast/StructLayout.h"
#include "ast/StructField.h"

using namespace cish::ast;


TEST(StructLayoutTest, emptyStructsThrows)
{
    StructLayout layout("empty_lol");
    ASSERT_THROW(layout.finalize(), EmptyStructException);
}

TEST(StructLayoutTest, cannotAddFieldAfterFinalizing)
{
    StructLayout layout("empty_lol");
    layout.addField(new StructField(TypeDecl::INT, "A"));
    layout.finalize();
    ASSERT_THROW(layout.addField(new StructField(TypeDecl::INT, "B")), StructFinalizedException);
}

TEST(StructLayoutTest, fieldNamesMustBeUnique)
{
    StructLayout l("s");
    l.addField(new StructField(TypeDecl::INT, "foo"));
    ASSERT_THROW(l.addField(new StructField(TypeDecl::FLOAT, "foo")),
                 FieldAlreadyDeclaredException);
}

TEST(StructLayoutTest, simpleValidStruct)
{
    StructLayout l("s");
    ASSERT_NO_THROW(l.addField(new StructField(TypeDecl::INT, "my_int")));
    ASSERT_NO_THROW(l.addField(new StructField(TypeDecl::FLOAT, "my_float")));
}

TEST(StructLayoutTest, offset)
{
    StructLayout layout("s");
    layout.addField(new StructField(TypeDecl::INT, "my_int"));
    layout.addField(new StructField(TypeDecl::BOOL, "my_bool"));
    layout.addField(new StructField(TypeDecl::FLOAT, "my_float"));
    layout.addField(new StructField(TypeDecl::getPointer(TypeDecl::VOID), "my_ptr"));

    ASSERT_EQ(0, layout.getField("my_int")->getOffset());
    ASSERT_EQ(4, layout.getField("my_bool")->getOffset());
    ASSERT_EQ(5, layout.getField("my_float")->getOffset());
    ASSERT_EQ(9, layout.getField("my_ptr")->getOffset());
}

TEST(StructLayoutTest, size)
{
    StructLayout layout("s");
    layout.addField(new StructField(TypeDecl::INT, "my_int"));
    layout.addField(new StructField(TypeDecl::BOOL, "my_bool"));
    layout.addField(new StructField(TypeDecl::FLOAT, "my_float"));
    layout.addField(new StructField(TypeDecl::getPointer(TypeDecl::VOID), "my_ptr"));

    ASSERT_EQ(13, layout.getSize());
}

TEST(StructLayoutTest, invalidFieldLookupThrows)
{
    StructLayout layout("s"); 
    layout.addField(new StructField(TypeDecl::INT, "foo"));
    ASSERT_THROW(layout.getField("lol"), NoSuchFieldException);
}

TEST(StructLayoutTest, canRetrieveFields)
{
    StructLayout layout("s");
    layout.addField(new StructField(TypeDecl::INT, "intfoo"));
    layout.addField(new StructField(TypeDecl::CHAR, "charfoo"));
    layout.finalize();

    const StructField *intField = layout.getField("intfoo");
    ASSERT_EQ("intfoo", intField->getName());
    ASSERT_EQ(TypeDecl::INT, intField->getType().getType());
    ASSERT_EQ(0, intField->getOffset());

    const StructField *charField = layout.getField("charfoo");
    ASSERT_EQ("charfoo", charField->getName());
    ASSERT_EQ(TypeDecl::CHAR, charField->getType().getType());
    ASSERT_EQ(4, charField->getOffset());
}

TEST(StructLayoutTest, structMustHaveName)
{
    ASSERT_ANY_THROW(StructLayout l(""));
}

TEST(StructLayoutTest, fieldsMustHaveName)
{
    StructLayout l("s"); 
    ASSERT_ANY_THROW(l.addField(new StructField(TypeDecl::INT, "")));
}

