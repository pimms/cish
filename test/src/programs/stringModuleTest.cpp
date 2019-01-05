#include <gtest/gtest.h>

#include "module/string/stringModule.h"
#include "../TestHelpers.h"


TEST(StringModuleTest, memchrReturnsNullIfNotFound)
{
    assertExitCode(
        "#include <string.h>"
        "int main() {"
        "   return memchr(\"abcd\", 'e', 4);"
        "}", 0
    );
}

TEST(StringModuleTest, memchrReturnsFirstOccurrence)
{
    assertExitCode(
        "#include <string.h>"
        "const char *str = \"abba\";"
        "int main() {"
        "   return (int)memchr(str, 'a', 4) - (int)str;"
        "}", 0
    );
}

