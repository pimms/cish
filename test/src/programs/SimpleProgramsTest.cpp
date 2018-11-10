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

TEST(SimpleProgramsTest, multipleDeclarationsWithDefinitionAfterCall)
{
    const std::string source =
        "int foo(); "
        "int foo(); "
        "int main() { return foo(); } "
        "int foo() { return 2; } ";
    assertExitCode(source, 2);
}

TEST(SimpleProgramsTest, renamingParametersInFuncDef)
{
    const std::string source =
        "int foo(int a); "
        "int main() { return foo(5); } "
        "int foo(int b) { return b; } ";
    assertExitCode(source, 5);
}

TEST(SimpleProgramsTest, oneLineIf)
{
    const std::string source =
        "int foo(int n) {"
        "   if (n >= 5)"
        "       return 5;"
        "   return 0;"
        "}"
        "int main() {"
        "   return foo(6) + foo(4);"
        "}";
    assertExitCode(source, 5);
}

TEST(SipmleProgramsTest, quiteABitOfIfLogic)
{
    // A whoole lot of nonsense logic
    const std::string source =
        "bool greaterThanTen(int n) {"
        "    if (n > 10) {"
        "        return true;"
        "    }"
        "    return false;"
        "}"
        "int fifteen() { return 15; }"
        "int foo() {"
        "    int result;"
        "    int var = 11;"
        "    if (greaterThanTen(fifteen() + var)) {"
        "        result = 5;"
        "        result = result + 5;"
        "    }"
        "    if (result > 4) {"
        "        return result;"
        "    }"
        "    return 0;"
        "}"
        "int main() { return foo(); }";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, basicElseBranch)
{
    const std::string source =
        "int getFiveOrTen(bool getFive) {"
        "   if (getFive) {"
        "       return 5;"
        "   } else {"
        "       return 10;"
        "   }"
        "}"
        "int main() {"
        "   return getFiveOrTen(true) + getFiveOrTen(false);"
        "}";
    assertExitCode(source, 15);
}

TEST(SimpleProgramsTest, ifElseBranch)
{
    const std::string source =
        "int main()"
        "{"
        "    int a = 3;"
        "    if (a == 1) {"
        "        return 0;"
        "    } else if (a == 2) {"
        "        return 0;"
        "    } else if (a == 3) {"
        "        return 3;"
        "    } else {"
        "        return 5;"
        "    }"
        "}";
    assertExitCode(source, 3);
}

TEST(SimpleProgramsTest, nestedIf) {
    const std::string source =
        "int main() {"
        "    int a = 3;"
        "    if (a == 3) {"
        "        if (a == 3)"
        "            if (a == 0) {"
        "                return 0;"
        "            } else return 1;"
        "    }"
        "    return 0;"
        "}";
    assertExitCode(source, 1);
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

TEST(SimpleProgramsTest, declaredFunctionsMustBeDefined)
{
    assertCompilationFailure(
        "void foo();"
        "int main() {return 0;} "
    );
}

TEST(SimpleProgramsTest, redeclarationWithDifferentSignatureIllegal)
{
    assertCompilationFailure(
        "void foo(); "
        "int foo(); "
        "void main() {}"
    );
    assertCompilationFailure(
        "void foo(); "
        "void foo(int a); "
        "void main() {}"
    );
    assertCompilationFailure(
        "void foo(int a); "
        "void foo(float a); "
        "void main() {}"
    );
}

TEST(SimpleProgramsTest, definitionWithDifferentSignatureIllegal)
{
    assertCompilationFailure(
        "void foo(); "
        "int foo() { return 0; } "
        "void main() {}"
    );
    assertCompilationFailure(
        "void foo(); "
        "void foo(int a) {} "
        "void main() {}"
    );
    assertCompilationFailure(
        "void foo(int a); "
        "void foo(float a) {} "
        "void main() {}"
    );
}

TEST(SimpleProgramsTest, declarationsInIfBranchUnavailableOutside)
{
    assertCompilationFailure(
        "void foo() { "
        "   if (1 == 5) { "
        "       int a = 15; "
        "   int b = a; "
        "} "
        "void main() {}"
    );
}
