#include <gtest/gtest.h>

#include "ast/LineNumberResolver.h"

using cish::ast::LineNumberResolver;

TEST(LineNumberResolverTest, emptySourceResolvesEverythingToLine1)
{
    LineNumberResolver resolver("");
    ASSERT_EQ(1, resolver.resolveCharIndex(0));
    ASSERT_EQ(1, resolver.resolveCharIndex(1));
    ASSERT_EQ(1, resolver.resolveCharIndex(100));
    ASSERT_EQ(1, resolver.resolveCharIndex((uint32_t)~0));
}

TEST(LineNumberResolverTest, oneLineSourceResolvesEverythingToLine1)
{
    LineNumberResolver resolver("int main() { return 0; }");
    ASSERT_EQ(1, resolver.resolveCharIndex(0));
    ASSERT_EQ(1, resolver.resolveCharIndex(1));
    ASSERT_EQ(1, resolver.resolveCharIndex(100));
    ASSERT_EQ(1, resolver.resolveCharIndex((uint32_t)~0));
}

TEST(LineNumberResolverTest, newlinesCountAsLastCharOnLine)
{
    LineNumberResolver resolver("abc\ndef\r\nghi");

    ASSERT_EQ(1, resolver.resolveCharIndex(3));
    ASSERT_EQ(2, resolver.resolveCharIndex(4));

    ASSERT_EQ(2, resolver.resolveCharIndex(8));
    ASSERT_EQ(3, resolver.resolveCharIndex(9));
}

TEST(LineNumberResolverTest, consecutiveNewlines)
{
    LineNumberResolver resolver("\n\n\n\n");
    ASSERT_EQ(1, resolver.resolveCharIndex(0));
    ASSERT_EQ(2, resolver.resolveCharIndex(1));
    ASSERT_EQ(3, resolver.resolveCharIndex(2));
    ASSERT_EQ(4, resolver.resolveCharIndex(3));
}

TEST(LineNumberResolverTest, simpleTwoLineTest)
{
    LineNumberResolver resolver("a\nb");
    ASSERT_EQ(1, resolver.resolveCharIndex(0));
    ASSERT_EQ(1, resolver.resolveCharIndex(1));
    ASSERT_EQ(2, resolver.resolveCharIndex(2));
}

TEST(LineNumberResolverTest, charsBeyondLastResolvesToLastLine)
{
    LineNumberResolver resolver("a\nbbb\nc");
    ASSERT_EQ(3, resolver.resolveCharIndex(6));
    ASSERT_EQ(3, resolver.resolveCharIndex((uint32_t)~0));
}
