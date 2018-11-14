/**
 * This test-file includes tests that indirectly tests the internals
 * of the implementation. A failure here will probably not tell you
 * much of what has gone wrong, but it should be able to indicate that
 * *something* has gone wrong.. :)
 */

#include <gtest/gtest.h>

#include "../TestHelpers.h"
#include "vm/VirtualMachine.h"

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
TEST(SimpleProgramsTest, exitCodeReturnedFromMain) {
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

TEST(SimpleProgramsTest, nestedIf)
{
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

TEST(SimpleProgramsTest, simpleForCounter)
{
    const std::string source =
        "void foo() {}"
        "int main()"
        "{"
        "    int var = 0;"
        "    for (int i=0; i<10; i=i+1)"
        "        var = var + 1;"
        "    return var;"
        "}";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, forLoopOnlyWithConditional)
{
    const std::string source =
        "int global = 10;"
        "bool foo() { global = global - 1; return global >= 0; }"
        "int main() {"
        "    int n = 0;"
        "    for (; foo();)"
        "        n = n + 1;"
        "    return n;"
        "}";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, infiniteForLoop)
{
    // We're not REALLY able to verify this, but we know for certain
    // that if the VM survives 200 statement-iterations, that we've
    // not returned... :)
    const std::string source = "int main() { for (;;); }";
    VmPtr vm = createVm(source);

    for (int i=0; i<200; i++) {
        ASSERT_TRUE(vm->isRunning());
        vm->executeNextStatement();
    }
    ASSERT_TRUE(vm->isRunning());
}

TEST(SimpleProgramsTest, nestedLoops)
{
    const std::string source =
        "int main() {"
        "    int n = 0;"
        "    for (int i=0; i<5; i=i+1) {"
        "        for (int j=0; j<5; j=j+1) {"
        "            n = n+1;"
        "        }"
        "    }"
        "   return n;"
        "}";
    assertExitCode(source, 25);
}

TEST(SimpleProgramsTest, whileLoop)
{
    const std::string source =
        "int main() {"
        "   int n = 0;"
        "   while (n < 5) { n = n + 1; }"
        "   return n;"
        "}";
    assertExitCode(source, 5);
}

TEST(SimpleProgramsTest, doWhileLoop)
{
    const std::string source =
        "int main() {"
        "    int n = 0;"
        "    do { n = n + 1; } while (n < 5);"
        "    return n;"
        "}";
    assertExitCode(source, 5);
}

TEST(SimpleProgramsTest, doWhileLoopOnlyRunOnce)
{
    const std::string source =
        "int main() {"
        "    int n = 0;"
        "    do { n = n + 1; } while (false);"
        "    return n;"
        "}";
    assertExitCode(source, 1);
}


TEST(SimpleProgramsTest, postfixIncrement)
{
    const std::string source =
        "int main() {"
        "    int n = 0;"
        "    return n++;"
        "}";
    assertExitCode(source, 0);
}

TEST(SimpleProgramsTest, prefixIncrement)
{
    const std::string source =
        "int main() {"
        "    int n = 0;"
        "    return ++n;"
        "}";
    assertExitCode(source, 1);
}

TEST(SimpleProgramsTest, postfixDecrement)
{
    const std::string source =
        "int main() {"
        "    int n = 10;"
        "    return n--;"
        "}";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, prefixDecrement)
{
    const std::string source =
        "int main() {"
        "    int n = 10;"
        "    return --n;"
        "}";
    assertExitCode(source, 9);
}

TEST(SimpleProgramsTest, standaloneIncrements)
{
    const std::string source =
        "int main() {"
        "   int n = 0;"
        "   n++;"
        "   return n;"
        "}";
    assertExitCode(source, 1);
}


TEST(SimpleProgramsTest, forLoopWithIncrement)
{
    const std::string source =
        "int main() {"
        "   int n = 0;"
        "   for (int i=0; i<10; i++) {"
        "       n++; n--; ++n;"
        "   }"
        "   return n;"
        "}";
    assertExitCode(source, 10);
}

TEST(SimpleProgramsTest, assigningFromConstant)
{
    const std::string source =
        "const int CONSTANT = 100;"
        "int main() {"
        "   int var = CONSTANT / 2;"
        "   return var;"
        "}";
    assertExitCode(source, 50);
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

TEST(SimpleProgramsTest, reassigningConstVariable)
{
    assertCompilationFailure(
        "void main() {"
        "   const int const = 10;"
        "   const++;"
        "}"
    );
}

