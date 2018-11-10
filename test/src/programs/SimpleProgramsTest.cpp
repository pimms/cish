/**
 * This test-file includes tests that indirectly tests the internals
 * of the implementation. A failure here will probably not tell you
 * much of what has gone wrong, but it should be able to indicate that
 * *something* has gone wrong.. :)
 */

#include <gtest/gtest.h>

#include "../TestHelpers.h"

using namespace cish::vm;
using namespace cish::ast;


void assertExitCode(const std::string &source, int expectedExitCode)
{
    VmPtr vm = createVm(source);

    while (vm->isRunning()) {
        vm->executeNextStatement();
    }

    ASSERT_EQ(expectedExitCode, vm->getExitCode());
}


TEST(SimpleProgramsTest, returningFooFromMain)
{
    const std::string source =
        "int foo() { return 9 + 1; } "
        "int main() { return foo() - 5; }";
    assertExitCode(source, 5);
}

TEST(SimpleProgramsTest, returningTwiceFromFooAndMain)
{
    const std::string source =
        "int foo() { return 5; return 100; } "
        "int main() { return foo(); return 99; } ";
    assertExitCode(source, 5);
}

