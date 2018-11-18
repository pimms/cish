#include <gtest/gtest.h>

#include "ast/StringEscape.h"

using namespace cish::ast::string;


TEST(StringEscapeTest, testStringEscaping)
{
    const std::string escaped = "\\n \\t abc \\\"";

    const std::string expected = "\n \t abc \"";
    const std::string actual = unescapeString(escaped);

    ASSERT_EQ(expected, actual);
}