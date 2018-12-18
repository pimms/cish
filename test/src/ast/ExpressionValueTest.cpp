#include <gtest/gtest.h>

#include "ast/ExpressionValue.h"

using namespace cish::ast;


TEST(ExpressionValueTest, FloatIsCorrectlyIdentifiedFromString)
{
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("1.5").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("1.5500").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("1.5500f").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("1.5500F").getIntrinsicType().getType());

    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("10.5").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("10.5500").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("10.5500f").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue("10.5500F").getIntrinsicType().getType());

    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue(".5").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue(".5500").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue(".5500f").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::FLOAT, ExpressionValue(".5500F").getIntrinsicType().getType());
}

TEST(ExpressionValueTest, IntegerIsCorrectlyIdentifiedFromString)
{
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("1").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("10").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("101").getIntrinsicType().getType());
}

TEST(ExpressionValueTest, HexadecimalIntegerIsCorrectlyIdentifiedFromString)
{
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("0x10c").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("0x0").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, ExpressionValue("0xffFFffFF").getIntrinsicType().getType());
}

TEST(ExpressionValueTest, BoolIsCorrectlyIdentifiedFromString)
{
    ASSERT_EQ(TypeDecl::BOOL, ExpressionValue("true").getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::BOOL, ExpressionValue("false").getIntrinsicType().getType());
}

TEST(ExpressionValueTest, charactersAreCorrectlyResolved)
{
    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'a'").getIntrinsicType().getType());
    ASSERT_EQ('a', ExpressionValue("'a'").get<char>());

    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'z'").getIntrinsicType().getType());
    ASSERT_EQ('z', ExpressionValue("'z'").get<char>());

    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'\\0'").getIntrinsicType().getType());
    ASSERT_EQ('\0', ExpressionValue("'\\0'").get<char>());

    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'.'").getIntrinsicType().getType());
    ASSERT_EQ('.', ExpressionValue("'.'").get<char>());

    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'\\''").getIntrinsicType().getType());
    ASSERT_EQ('\'', ExpressionValue("'\\''").get<char>());

    ASSERT_EQ(TypeDecl::CHAR, ExpressionValue("'\"'").getIntrinsicType().getType());
    ASSERT_EQ('"', ExpressionValue("'\"'").get<char>());
}


TEST(ExpressionValueTest, IntrinsicFloat_GetAsFloatTypes)
{
    ASSERT_NEAR(10.4f, ExpressionValue("10.4").get<float>(), 0.001f);
    ASSERT_NEAR(.4, ExpressionValue(".4F").get<double>(), 0.001f);
    ASSERT_NEAR(100.0, ExpressionValue("100.0").get<double>(), 0.001f);
}

TEST(ExpressionValueTest, IntrinsicFloat_GetAsIntTypes)
{
    ASSERT_EQ(10, ExpressionValue("10.8").get<int8_t>());
    ASSERT_EQ(10, ExpressionValue("10.8").get<int16_t>());
    ASSERT_EQ(10, ExpressionValue("10.8").get<int32_t>());

    ASSERT_EQ(44,  ExpressionValue("300.1").get<int8_t>());
    ASSERT_EQ(300, ExpressionValue("300.1").get<int16_t>());
    ASSERT_EQ(300, ExpressionValue("300.1").get<int32_t>());
}


TEST(ExpressionValueTest, IntrinsicInt_GetAsIntTypes)
{
    ASSERT_EQ(-57, ExpressionValue("199").get<int8_t>());
    ASSERT_EQ(199, ExpressionValue("199").get<uint8_t>());
    ASSERT_EQ(199, ExpressionValue("199").get<int16_t>());
    ASSERT_EQ(199, ExpressionValue("199").get<int32_t>());

    ASSERT_EQ(true, ExpressionValue("199").get<bool>());
    ASSERT_EQ(false, ExpressionValue("0").get<bool>());
}

TEST(ExpressionValueTest, IntrinsicHex_GetAsIntTypes)
{
    ASSERT_EQ(-57, ExpressionValue("0xC7").get<int8_t>());
    ASSERT_EQ(199, ExpressionValue("0xc7").get<uint8_t>());
    ASSERT_EQ(199, ExpressionValue("0xc7").get<int16_t>());
    ASSERT_EQ(199, ExpressionValue("0xc7").get<int32_t>());

    ASSERT_EQ(true, ExpressionValue("0xC7").get<bool>());
    ASSERT_EQ(false, ExpressionValue("0x0000").get<bool>());
}



TEST(ExpressionValueTest, IntrinsicInt_GetAsFloatTypes)
{
    ASSERT_NEAR(199.f, ExpressionValue("199").get<float>(), 0.001f);
    ASSERT_NEAR(199.f, ExpressionValue("199").get<double>(), 0.001f);
}


TEST(ExpressionValueTest, IntrinsicBool_GetAsBool)
{
    ASSERT_EQ(true, ExpressionValue("true").get<bool>());
    ASSERT_EQ(false, ExpressionValue("false").get<bool>());
}

TEST(ExpressionValueTest, IntrinsicBool_GetAsInt)
{
    ASSERT_EQ(1, ExpressionValue("true").get<int>());
    ASSERT_EQ(0, ExpressionValue("false").get<int>());
}

TEST(ExpressionValueTest, IntrinsicBool_GetAsFloat)
{
    ASSERT_NEAR(1.0f, ExpressionValue("true").get<float>(), 0.001f);
    ASSERT_NEAR(0.0f, ExpressionValue("false").get<float>(), 0.001f);
}


TEST(ExpressionValueTest, InvalidInputThrows)
{
    // Will support at some point.. :)
    ASSERT_THROW(ExpressionValue("199L"), ExpressionTypeException);
    ASSERT_THROW(ExpressionValue("199l"), ExpressionTypeException);
    ASSERT_THROW(ExpressionValue("199U"), ExpressionTypeException);
    ASSERT_THROW(ExpressionValue("0x"), ExpressionTypeException);
    ASSERT_THROW(ExpressionValue("0X0"), ExpressionTypeException);

    // Not sure how to handle
    ASSERT_THROW(ExpressionValue("\"heisann\""), ExpressionTypeException);

    // Let's just be sure
    ASSERT_THROW(ExpressionValue("var"), ExpressionTypeException);
}


TEST(ExpressionValueTest, IntCastedToFloat)
{
    ExpressionValue original = ExpressionValue("100");
    ExpressionValue casted = original.cast<float>();

    ASSERT_EQ(TypeDecl::FLOAT, casted.getIntrinsicType().getType());
    ASSERT_NEAR(100.0, casted.get<float>(), 0.001f);
}

TEST(ExpressionValueTest, FloatCastedToShort)
{
    ExpressionValue original = ExpressionValue("100.5");
    ExpressionValue casted = original.cast<short>();

    ASSERT_EQ(TypeDecl::SHORT, casted.getIntrinsicType().getType());
    ASSERT_EQ(100, casted.get<short>());
}

TEST(ExpressionValueTest, IntCastedToCharCastedToInt)
{
    ExpressionValue original = ExpressionValue("300");

    ExpressionValue castedOnce = original.cast<char>();
    ASSERT_EQ(TypeDecl::CHAR, castedOnce.getIntrinsicType().getType());
    ASSERT_EQ(44, castedOnce.get<char>());

    ExpressionValue castedTwice = castedOnce.cast<int>();
    ASSERT_EQ(TypeDecl::INT, castedTwice.getIntrinsicType().getType());
    ASSERT_EQ(44, castedTwice.get<int>());
}

TEST(ExpressionValue, IntCastedToBoolCastedToInt)
{
    ExpressionValue original = ExpressionValue("300");

    ExpressionValue castedOnce = original.cast<bool>();
    ASSERT_EQ(TypeDecl::BOOL, castedOnce.getIntrinsicType().getType());
    ASSERT_EQ(true, castedOnce.get<bool>());

    ExpressionValue castedTwice = castedOnce.cast<int>();
    ASSERT_EQ(TypeDecl::INT, castedTwice.getIntrinsicType().getType());
    ASSERT_EQ(1, castedTwice.get<int>());
}

