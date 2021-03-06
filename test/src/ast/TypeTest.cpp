#include <gtest/gtest.h>

#include "ast/Type.h"
#include "ast/AstNodes.h"
#include "ast/DeclarationContext.h"
#include "ast/StructLayout.h"
#include "ast/StructField.h"

using namespace cish::ast;


#define tdptr(x) TypeDecl::getPointer(x)
#define tdconst(x) TypeDecl::getConst(x)


TEST(TypeTest, sizeTest)
{
    ASSERT_EQ(0, TypeDecl(TypeDecl::VOID).getSize());
    ASSERT_EQ(1, TypeDecl(TypeDecl::BOOL).getSize());
    ASSERT_EQ(1, TypeDecl(TypeDecl::CHAR).getSize());
    ASSERT_EQ(2, TypeDecl(TypeDecl::SHORT).getSize());
    ASSERT_EQ(4, TypeDecl(TypeDecl::INT).getSize());
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
    ASSERT_EQ(TypeDecl(TypeDecl::FLOAT), TypeDecl::getFromString("float"));
    ASSERT_EQ(TypeDecl(TypeDecl::DOUBLE), TypeDecl::getFromString("double"));
}

TEST(TypeTest, resolveConstFromTokens)
{
    DeclarationContext dc;
    ASSERT_EQ(tdconst(TypeDecl::VOID), TypeDecl::getFromTokens(&dc, {"const", "void"}));
    ASSERT_EQ(tdconst(TypeDecl::BOOL), TypeDecl::getFromTokens(&dc, {"const", "bool"}));
    ASSERT_EQ(tdconst(TypeDecl::CHAR), TypeDecl::getFromTokens(&dc, {"const", "char"}));
    ASSERT_EQ(tdconst(TypeDecl::SHORT), TypeDecl::getFromTokens(&dc, {"const", "short"}));
    ASSERT_EQ(tdconst(TypeDecl::INT), TypeDecl::getFromTokens(&dc, {"const", "int"}));
    ASSERT_EQ(tdconst(TypeDecl::FLOAT), TypeDecl::getFromTokens(&dc, {"const", "float"}));
    ASSERT_EQ(tdconst(TypeDecl::DOUBLE), TypeDecl::getFromTokens(&dc, {"const", "double"}));
}

TEST(TypeTest, resolvePointerFromString)
{
    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("void*").getType());
    ASSERT_EQ(TypeDecl::VOID, TypeDecl::getFromString("void*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("bool*").getType());
    ASSERT_EQ(TypeDecl::BOOL, TypeDecl::getFromString("bool*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("char*").getType());
    ASSERT_EQ(TypeDecl::CHAR, TypeDecl::getFromString("char*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("short*").getType());
    ASSERT_EQ(TypeDecl::SHORT, TypeDecl::getFromString("short*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("int*").getType());
    ASSERT_EQ(TypeDecl::INT, TypeDecl::getFromString("int*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("float*").getType());
    ASSERT_EQ(TypeDecl::FLOAT, TypeDecl::getFromString("float*").getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::POINTER, TypeDecl::getFromString("double*").getType());
    ASSERT_EQ(TypeDecl::DOUBLE, TypeDecl::getFromString("double*").getReferencedType()->getType());
}

TEST(TypeTest, resolvePointerPointerFromString)
{
    auto type = TypeDecl::getFromString("int**");
    ASSERT_EQ(TypeDecl::POINTER, type.getType());
    ASSERT_EQ(TypeDecl::POINTER, type.getReferencedType()->getType());
    ASSERT_EQ(TypeDecl::INT, type.getReferencedType()->getReferencedType()->getType());
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

TEST(TypeTest, referencedTypeIsCorrectlyCopied)
{
    TypeDecl ptr = TypeDecl::getPointer(TypeDecl::Type::INT);
    TypeDecl copy = ptr;

    ASSERT_NE(nullptr, ptr.getReferencedType());
    ASSERT_NE(nullptr, copy.getReferencedType());
    ASSERT_EQ(ptr.getReferencedType(), copy.getReferencedType());
}

TEST(TypeTest, primitivesHasNoReferencedType)
{
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::VOID).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::BOOL).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::CHAR).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::SHORT).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::INT).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::FLOAT).getReferencedType());
    ASSERT_ANY_THROW(TypeDecl(TypeDecl::DOUBLE).getReferencedType());
}

TEST(TypeTest, pointersMustBeConstructedExplicitly)
{
    ASSERT_ANY_THROW(TypeDecl type(TypeDecl::POINTER));
}

TEST(TypeTest, pointerTypesReferenceCorrectly)
{
    TypeDecl pointer = TypeDecl::getPointer(TypeDecl::INT);
    ASSERT_EQ(TypeDecl::INT, pointer.getReferencedType()->getType());
    ASSERT_ANY_THROW(pointer.getReferencedType()->getReferencedType());
}

TEST(TypeTest, copiedPointerTypesReferenceCorrectly)
{
    TypeDecl copy;
    {
        TypeDecl pointer = TypeDecl::getPointer(TypeDecl::INT);
        copy = pointer;
    }

    ASSERT_EQ(TypeDecl::INT, copy.getReferencedType()->getType());
    ASSERT_ANY_THROW(copy.getReferencedType()->getReferencedType());
}

TEST(TypeTest, pointerToPointerTypesReferenceCorrectly)
{
    TypeDecl ptr = TypeDecl::getPointer(TypeDecl::INT);
    TypeDecl ptrPtr = TypeDecl::getPointer(ptr);

    ASSERT_EQ(TypeDecl::POINTER, ptrPtr.getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::INT, ptrPtr.getReferencedType()->getReferencedType()->getType());
    ASSERT_ANY_THROW(ptrPtr.getReferencedType()->getReferencedType()->getReferencedType());
}

TEST(TypeTest, copierPointerToPointerTypesReferenceCorrectly)
{
    TypeDecl copy;
    {
        TypeDecl ptr = TypeDecl::getPointer(TypeDecl::INT);
        TypeDecl ptrPtr = TypeDecl::getPointer(ptr);
        copy = ptrPtr;
    }

    ASSERT_EQ(TypeDecl::POINTER, copy.getReferencedType()->getType());

    ASSERT_EQ(TypeDecl::INT, copy.getReferencedType()->getReferencedType()->getType());
    ASSERT_ANY_THROW(copy.getReferencedType()->getReferencedType()->getReferencedType());
}


TEST(TypeTest, deepPointerEqualityTest)
{
    TypeDecl rawInt = TypeDecl::INT;
    TypeDecl intPtr = TypeDecl::getPointer(TypeDecl::INT);
    TypeDecl floatPtr = TypeDecl::getPointer(TypeDecl::FLOAT);
    TypeDecl intPtrPtr = TypeDecl::getPointer(intPtr);
    TypeDecl floatPtrPtr = TypeDecl::getPointer(floatPtr);

    ASSERT_TRUE(rawInt == rawInt);
    ASSERT_FALSE(rawInt == intPtr);
    ASSERT_FALSE(rawInt == intPtrPtr);
    ASSERT_FALSE(rawInt == floatPtr);
    ASSERT_FALSE(rawInt == floatPtrPtr);

    ASSERT_FALSE(intPtr == rawInt);
    ASSERT_TRUE(intPtr == intPtr);
    ASSERT_FALSE(intPtr == intPtrPtr);
    ASSERT_FALSE(intPtr == floatPtr);
    ASSERT_FALSE(intPtr == floatPtrPtr);

    ASSERT_FALSE(intPtrPtr == rawInt);
    ASSERT_FALSE(intPtrPtr == intPtr);
    ASSERT_TRUE(intPtrPtr == intPtrPtr);
    ASSERT_FALSE(intPtrPtr == floatPtr);
    ASSERT_FALSE(intPtrPtr == floatPtrPtr);

    ASSERT_FALSE(floatPtr == rawInt);
    ASSERT_FALSE(floatPtr == intPtr);
    ASSERT_FALSE(floatPtr == intPtrPtr);
    ASSERT_TRUE(floatPtr == floatPtr);
    ASSERT_FALSE(floatPtr == floatPtrPtr);

    ASSERT_FALSE(floatPtrPtr == rawInt);
    ASSERT_FALSE(floatPtrPtr == intPtr);
    ASSERT_FALSE(floatPtrPtr == intPtrPtr);
    ASSERT_FALSE(floatPtrPtr == floatPtr);
    ASSERT_TRUE(floatPtrPtr == floatPtrPtr);
}

TEST(TypeTest, pointerTypeDeclWithPointerTypeIsShallow)
{
    TypeDecl intPtr = TypeDecl::getPointer(TypeDecl::INT);
    TypeDecl voidPtr = TypeDecl::getPointer(TypeDecl::VOID);

    ASSERT_TRUE(intPtr == TypeDecl::POINTER);
    ASSERT_TRUE(voidPtr == TypeDecl::POINTER);
    ASSERT_FALSE(intPtr == voidPtr);
}

TEST(TypeTest, getFromTokens)
{
    DeclarationContext dc;

    TypeDecl constIntPtrPtr = TypeDecl::INT;
    constIntPtrPtr.setConst(true);
    constIntPtrPtr = TypeDecl::getPointer(constIntPtrPtr);
    constIntPtrPtr = TypeDecl::getPointer(constIntPtrPtr);
    std::vector<std::string> cippTokens = {"const", "int", "*", "*"};
    ASSERT_EQ(constIntPtrPtr, TypeDecl::getFromTokens(&dc, cippTokens));

    TypeDecl intPtr = TypeDecl::getPointer(TypeDecl::INT);
    std::vector<std::string> ipTokens = {"int", "*"};
    ASSERT_EQ(intPtr, TypeDecl::getFromTokens(&dc, ipTokens));

    std::vector<std::string> iTokens = { "int" };
    ASSERT_EQ(TypeDecl(TypeDecl::INT), TypeDecl::getFromTokens(&dc, iTokens));
}

TEST(TypeTest, getStructFromTokens)
{
    DeclarationContext dc;

    StructLayout s1("s1");
    s1.addField(new StructField(TypeDecl::INT, "n"));

    StructLayout s2("s2");
    s2.addField(new StructField(TypeDecl::INT, "n"));

    dc.declareStruct(&s1);
    dc.declareStruct(&s2);

    auto raw = TypeDecl::getFromTokens(&dc, {"struct", "s1"});
    auto constant = TypeDecl::getFromTokens(&dc, {"const", "struct", "s1"});
    auto ptr = TypeDecl::getFromTokens(&dc, {"struct", "s2", "*"});
    auto constPtr = TypeDecl::getFromTokens(&dc, {"const", "struct", "s1", "*"});

    ASSERT_EQ(TypeDecl::STRUCT, raw.getType());
    ASSERT_EQ(dc.getStruct("s1"), raw.getStructLayout());
    ASSERT_FALSE(raw.isConst());

    ASSERT_EQ(TypeDecl::STRUCT, constant.getType());
    ASSERT_EQ(dc.getStruct("s1"), constant.getStructLayout());
    ASSERT_TRUE(constant.isConst());

    ASSERT_EQ(TypeDecl::POINTER, ptr.getType());
    ASSERT_EQ(TypeDecl::STRUCT, ptr.getReferencedType()->getType());
    ASSERT_EQ(dc.getStruct("s2"), ptr.getReferencedType()->getStructLayout());
    ASSERT_FALSE(ptr.getReferencedType()->isConst());

    ASSERT_EQ(TypeDecl::POINTER, constPtr.getType());
    ASSERT_EQ(constant, *constPtr.getReferencedType());
}

TEST(TypeTest, primitivesDoesntCareAboutConstInCasting)
{
    std::vector<TypeDecl::Type> primitives = {
        TypeDecl::BOOL,
        TypeDecl::CHAR,
        TypeDecl::SHORT,
        TypeDecl::INT,
        TypeDecl::FLOAT,
        TypeDecl::DOUBLE,
    };

    for (auto type: primitives) {
        ASSERT_TRUE(TypeDecl::getConst(type).castableTo(type));
        ASSERT_TRUE(TypeDecl(type).castableTo(TypeDecl::getConst(type)));
    }
}

TEST(TypeTest, castingConstPointersToMutablePointersNotAllowed)
{
    TypeDecl iPtr = TypeDecl::getPointer(TypeDecl::INT);
    TypeDecl ciPtr = TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::INT));

    ASSERT_TRUE(iPtr.castableTo(ciPtr));
    ASSERT_FALSE(ciPtr.castableTo(iPtr));
}

TEST(TypeTest, transparentAndOpaquePointersAreImplicitlyCastable)
{
    StructLayout structLayout("layout");
    structLayout.addField(new StructField(TypeDecl::INT, "n"));
    structLayout.finalize();

    std::vector<TypeDecl> types = {
        TypeDecl::getPointer(TypeDecl::BOOL),
        TypeDecl::getPointer(TypeDecl::CHAR),
        TypeDecl::getPointer(TypeDecl::SHORT),
        TypeDecl::getPointer(TypeDecl::INT),
        TypeDecl::getPointer(TypeDecl::FLOAT),
        TypeDecl::getPointer(TypeDecl::DOUBLE),
        TypeDecl::getPointer(TypeDecl::getStruct(&structLayout)),
    };

    TypeDecl voidPointer = TypeDecl::getPointer(TypeDecl::VOID);

    for (TypeDecl &type: types) {
        ASSERT_TRUE(type.castableTo(voidPointer));
        ASSERT_TRUE(voidPointer.castableTo(type));
    }
}


TEST(TypeTest, pointerNaming)
{
    EXPECT_EQ("int*", std::string(tdptr(TypeDecl::INT).getName()));
    EXPECT_EQ("const int*", std::string(tdptr(tdconst(TypeDecl::INT)).getName()));
    EXPECT_EQ("const void***", std::string(tdptr(tdptr(tdptr(tdconst(TypeDecl::VOID)))).getName()));
}

TEST(TypeTest, safeFromBufferOverflowInTypeName)
{
    TypeDecl type = TypeDecl::INT;

    for (int i=0; i<1000; i++) {
        type = tdptr(type);
    }

    std::string name = type.getName();
    ASSERT_EQ(98, name.length());
    ASSERT_EQ("...", name.substr(95));
}

