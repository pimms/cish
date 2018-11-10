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



/* VALID PROGRAMS */

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

TEST(SimpleProgramsTest, exitCodeDefaultsToZeroForVoidMain)
{
    const std::string source = "void main() {}";
    assertExitCode(source,0 );
}

TEST(SimpleProgramsTest, exitCodeReturnedFromMain)
{
    const std::string source = "int main() { return 15; }";
    assertExitCode(source, 15);
}

TEST(SimpleProgramsTest, returningAGlobalVariable)
{
    const std::string source =
        "int foo() { int a = 5; int b = 10; return a+b; } "
        "int a = foo(); "
        "int main() { return a; }";
    assertExitCode(source, 15);
}

TEST(SimpleProgramsTest, returningAParameter)
{
    const std::string source =
        "int foo(int a) { return a; } "
        "int main() { return foo(10); } ";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, changingAndReturningAParameter)
{
    const std::string source =
        "int addTwo(int a) { a = a + 2; return a; } "
        "int main() { "
        "   int a = 10; "
        "   a = addTwo(a); "
        "   return addTwo(a); "
        "} ";
    assertExitCode(source, 14);
}


/* COMPILATION FAILURES */

void assertCompilationFailure(const std::string &source)
{
    ASSERT_ANY_THROW(createAst(source));
}

TEST(SimpleProgramsTest, cannotReturnValueFromVoidFunction)
{
    assertCompilationFailure(
        "void foo() { return 5; } "
        "int main() {}"
    );
}

TEST(SipmleProgramsTest, cannotAssignVoidFunctionToVariable)
{
    assertCompilationFailure(
        "void foo() {} "
        "void main() { float a = foo(); }"
    );
}

TEST(SimpleProgramsTest, cannotReturnWithoutValueFromNonVoidFunction)
{
    assertCompilationFailure(
        "void main() { return 1; }"
    );
}

