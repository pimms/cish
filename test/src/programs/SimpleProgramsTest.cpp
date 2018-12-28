/**
 * This test-file includes tests that indirectly tests the internals
 * of the implementation. A failure here will probably not tell you
 * much of what has gone wrong, but it should be able to indicate that
 * *something* has gone wrong.. :)
 */

#include <gtest/gtest.h>

#include "../TestHelpers.h"
#include "vm/VirtualMachine.h"
#include "module/stdlib/stdlibModule.h"

using namespace cish::vm;
using namespace cish::ast;
using namespace cish::module;


 /* VALID PROGRAMS */


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

TEST(SimpleProgramsTest, negativeIntReturnedFromMain)
{
    const std::string source = "int main() { return -15; }";
    assertExitCode(source, -15);
}

TEST(SimpleProgramsTest, negativeFloatReturnedFromMain)
{
    assertExitCode("int main() { return -15.5f; }", -15);
    assertExitCode("int main() { return -.5f; }", 0);
    assertExitCode("int main() { return -1.f; }", -1);
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

TEST(SimpleProgramsTest, quiteABitOfIfLogic)
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
    vm->startSync();

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

TEST(SimpleProgramsTest, returningCharacters)
{
    assertExitCode("int main(){ return '\\n'; }", (int)'\n');
    assertExitCode("int main(){ return '\\t'; }", (int)'\t');
    assertExitCode("int main(){ return '0'; }", (int)'0');
    assertExitCode("int main(){ return 'a'; }", (int)'a');
    assertExitCode("int main(){ return '\\0'; }", 0);
}

TEST(SimpleProgramsTest, nullPointerIsNotEqualToValidPointer)
{
    assertExitCode(
        "int main() {"
        "   int raw = 15;"
        "   int *ptr1 = NULL;"
        "   int *ptr2 = &raw;"
        "   return ptr1 == ptr2;"
        "}", 0);
}

TEST(SimpleProgramsTest, pointersPointingToSameAddressAreEqual)
{
    assertExitCode(
        "int main() {"
        "   int raw = 15;"
        "   int *ptr1 = &raw;"
        "   int *ptr2 = &raw;"
        "   return ptr1 == ptr2;"
        "}", 1);
}

TEST(SimpleProgramsTest, addrofPointerEqualsPointerPointingToIt)
{
    assertExitCode(
        "int main() {"
        "   int *ptr1 = NULL;"
        "   int **ptr2 = &ptr1;"
        "   return &ptr1 == ptr2;"
        "}", 1);
}

TEST(SimpleProgramsTest, dereferencingPointerPointerEqualsPointer)
{
    assertExitCode(
        "int main() {"
        "   int *ptr1 = NULL;"
        "   int **ptr2 = &ptr1;"
        "   return ptr1 == *ptr2;"
        "}", 1);
}

TEST(SimpleProgramsTest, returningADereferencedPointer)
{
    const std::string source =
        "int main() {"
        "   int var = 100;"
        "   int *ptr = &var;"
        "   return *ptr;"
        "}";
    assertExitCode(source, 100);
}

TEST(SimpleProgramsTest, returningNullPointerValueEqualsZero)
{
    const std::string source =
        "int main() {"
        "   int *ptr = NULL;"
        "   return ptr;"
        "}";
    assertExitCode(source, 0);
}

TEST(SimpleProgramsTest, derefingPointerInsideFunction)
{
    const std::string source =
        "int foo(int *ptr) {"
        "   return *ptr + 10;"
        "}"
        "int main() {"
        "   int var = 16;"
        "   return foo(&var);"
        "}";
    assertExitCode(source, 26);
}

TEST(SimpleProgramsTest, incrementingDereferencedPointer)
{
    const std::string source =
        "int main() {"
        "   int var = 10;"
        "   int *ptr = &var;"
        "   *ptr = *ptr + 1;"
        "   return var;"
        "}";
    assertExitCode(source, 11);
}

TEST(SimpleProgramsTest, assigningToDereferencedPointer)
{
    const std::string source =
        "int main() {"
        "   int var = 10;"
        "   int *ptr = &var;"
        "   *ptr = 20;"
        "   return var;"
        "}";
    assertExitCode(source, 20);
}

TEST(SimpleProgramsTest, derefingCharFromString)
{
    assertExitCode(
        "int main() {"
        "   const char *str = \"abc\";"
        "   return *str;"
        "}", 'a');
    assertExitCode(
        "int main() {"
        "   const char *str = \"abc\";"
        "   str++;"
        "   return *str;"
        "}", 'b');
    assertExitCode(
        "int main() {"
        "   const char *str = \"abc\";"
        "   str++;"
        "   str++;"
        "   return *str;"
        "}", 'c');
    assertExitCode(
        "int main() {"
        "   const char *str = \"abc\";"
        "   str++;"
        "   str++;"
        "   str++;"
        "   return *str;"
        "}", 0);
}

TEST(SimpleProgramsTest, iteratingThroughString)
{
    const std::string source =
        "int main() {"
        "   int len = 0;"
        "   const char *str = \"abc\";"
        "   while (*str) {"
        "       len++;"
        "       str++;"
        "   }"
        "   return len;"
        "}";
    assertExitCode(source, 3);
}

TEST(SimpleProgramsTest, addingValuesOfString)
{
    const std::string source =
        "int main() {"
        "   int sum = 0;"
        "   const char *str = \"abc\";"
        "   do {"
        "       sum = sum + *str++;"
        "   } while (*str);"
        "   return sum;"
        "}";
    assertExitCode(source, 'a' + 'b' + 'c');
}

TEST(SimpleProgramsTest, tripleDerefAssignment)
{
    const std::string source =
        "int main() {"
        "   int core = 0;"
        "   int *ptr1 = &core;"
        "   int **ptr2 = &ptr1;"
        "   int ***ptr3 = &ptr2;"
        "   ***ptr3 = 9;"
        "   return core;"
        "}";
    assertExitCode(source, 9);
}

TEST(SimpleProgramsTest, passingStringAsArgument)
{
    const std::string source =
        "char firstChar(const char *str) { return *str; }"
        "int main() { return firstChar(\"1234\"); }";
    assertExitCode(source, (int)'1');
}

TEST(SimpleProgramsTest, paranthesisExpressions)
{
    // abuse the heck out of parantheses, this SHOULD only work
    // if we've done everything right thus far... :)
    const std::string source =
        "int echo(int n) { return (n); }"
        "int main() {"
        "   return ((5) * ((10) + (5))) - (10) + (echo((1)));"
        "}";
    assertExitCode(source, 66);
}

TEST(SimpleProgramsTest, bitwiseAnd)
{
    const std::string source =
        "int main() {"
        "   return 0x1f1f1f0f & 0x101010F0;"
        "}";
    assertExitCode(source, 0x10101000);
}

TEST(SimpleProgramsTest, bitwiseXor)
{
    const std::string source =
        "int main() {"
        "   return 0x0F11F ^ 0x0001FF1;"
        "}";
    assertExitCode(source, 0xEEEE);
}

TEST(SimpleProgramsTest, bitwiseOr)
{
    const std::string source =
        "int main() {"
        "   return 0x0f0f0f0f | 0xF0f0F0F0;"
        "}";
    assertExitCode(source, 0xFFFFFFFF);
}

TEST(SimpleProgramsTest, leftshift)
{
    assertExitCode(
        "int main() {"
        "   return 0x8F << 1;"
        "}", 0x11E);
    assertExitCode(
        "int main() {"
        "   return 0x44 << 2;"
        "}", 0x110);
    assertExitCode(
        "int main() {"
        "   return 0x33 << 7;"
        "}", 0x1980);
}

TEST(SimpleProgramsTest, rightShift)
{
    assertExitCode(
        "int main() {"
        "   return 0x8F >> 1;"
        "}", 0x47);
    assertExitCode(
        "int main() {"
        "   return 0x8 >> 1;"
        "}", 0x4);
    assertExitCode(
        "int main() {"
        "   return 0x1 >> 1;"
        "}", 0x0);
}

TEST(SimpleProgramsTest, simpleNegation)
{
    assertExitCode(
        "int main() {"
        "   return !true;"
        "}", 0);
    assertExitCode(
        "int main() {"
        "   return !false;"
        "}", 1);
    assertExitCode(
        "int main() {"
        "   return !-1;"
        "}", 0);
    assertExitCode(
        "int main() {"
        "   int n = 0xFF000000; "
        "   return !n;"
        "}", 0);
}

TEST(SimpleProgramsTest, doubleNegation)
{
    assertExitCode(
        "int main() {"
        "   return !!true;"
        "}", 1);
    assertExitCode(
        "int main() {"
        "   return !!false;"
        "}", 0);
    assertExitCode(
        "int main() {"
        "   return !!100;"
        "}", 1);
}

TEST(SimpleProgramsTest, onesCompliment)
{
    assertExitCode(
        "int main() {"
        "   return ~true;"
        "}", 0xfffffffe);
    assertExitCode(
        "int main() {"
        "   return ~0x10101010;"
        "}", 0xefefefef);
}

TEST(SimpleProgramsTest, sizeofTypeLiterals)
{
    std::map<std::string,int> typeMap = {
        {"bool", 1},
        {"char", 1},
        {"short", 2},
        {"int", 4},
        {"float", 4},
        {"double", 8},

        {"const bool", 1},
        {"const char*", 4},
        {"const short**", 4},
        {"int**", 4},
        {"const float*", 4},
        {"const double", 8},

        {"true", 1},
        {"false", 1},
        {"0x1020", 4},
        {"sizeof int", 4},
    };

    for (auto pair: typeMap) {
        assertExitCode("int main(){ return sizeof " + pair.first + "; }", pair.second);
        assertExitCode("int main(){ return sizeof (" + pair.first + "); }", pair.second);
    }
}

TEST(SimpleProgramsTest, sizeofVariables)
{
    assertExitCode(
        "int foo() { return 15; }"
        "int main() {"
        "   const int n = -39;"
        "   if (sizeof(n) != 4) return sizeof(n);"
        "   if (sizeof foo() != 4) return 2; "
        "   int a = 0;"
        "   int size = sizeof a;"
        "   if (size != 4) return 3;"
        "   return 0;"
        "}", 0);
}

TEST(SimpleProgramsTest, sizeofPresedence)
{
    assertExitCode("int main(){ return sizeof int + 1; }", 5);
    assertExitCode("int main(){ int n; return sizeof n + 1; }", 5);
    assertExitCode("int main(){ int n; return sizeof (n + 1); }", 4);
    assertExitCode("int main(){ return sizeof int == 4; }", 1);
    assertExitCode("int main(){ return sizeof int == 5; }", 0);
    assertExitCode("int main(){ int n; return sizeof (n == 5); }", 1);
}

TEST(SimpleProgramsTest, literalToBoolCast)
{
    assertExitCode("int main() { return (bool)10; }", 1);
    assertExitCode("int main() { return (bool)-10; }", 1);
    assertExitCode("int main() { return (bool)0; }", 0);

    assertExitCode("int main() { return (bool)10.f; }", 1);
    assertExitCode("int main() { return (bool).1f; }", 1);
    assertExitCode("int main() { return (bool)0.0f; }", 0);

    assertExitCode("int main() { return ((float)((bool)100) - .5f) * 11; }", 5);
}

TEST(SimpleProgramsTest, pointerToIntToPointerCast)
{
    assertExitCode(
        "int main() {"
        "   const int a = 15;"
        "   int b = 0xFF000000;"
        "   const int *aptr = &a;"
        ""
        "   int bptrVal = (int)aptr + sizeof int; "
        ""
        "   int *bptr = (int*)bptrVal;"
        "   *bptr = *bptr | 0x00EEDD00;"
        ""
        "   char *cptr = (char*)(bptr);"
        "   *cptr = 0xCC;"
        "   return b;"
        "}"
    , 0xFFEEDDCC);
}

TEST(SimpleProgramsTest, castAddrofExpression)
{
    assertExitCode(
        "int main() {"
        "   int value = 0x01020304;"
        "   return *(char*)&value;"
        "}",
        4
    );
}

TEST(SimpleProgramsTest, castDerefExpression)
{
    assertExitCode(
        "int main() {"
        "   int value = 0x55552323;"
        "   int *ptr = &value;"
        "   return (short)*ptr;"
        "}",
        0x2323
    );
}

TEST(SimpleProgramsTest, castPrecedence)
{
    assertExitCode("int main() { return (char)0xFFFF + 1; }", 0);
    assertExitCode("int main() { return (char)0xFFFF + 1; }", 0);
    assertExitCode(
        "int main() {"
        "    int value = 100;"
        "    int *ptr = &value;"
        "    short *nptr = (short*)ptr + 1;"
        "    int diff = (int)nptr - (int)&value;"
        "    return diff;"
        "}", 2);
}

TEST(SimpleProgramsTest, castingFunctionReturnValue)
{
    assertExitCode(
        "short foo() { return 0x2323; }"
        "int main() { return (char)foo(); }",
        0x23
    );
}

TEST(SimpleProgramsTest, simpleArithAss)
{
    assertExitCode(
        "int main() {"
        "   int val = 100;"
        "   val += 20;"     // 120
        "   val *= 3;"      // 360
        "   val /= 2;"      // 180
        "   val -= -9;"     // 189
        "   val -= 1;"      // 188
        "   return val;"
        "}", 188
    );
}

TEST(SimpleProgramsTest, subcriptSimple)
{
    assertExitCodeStdlib(
        "#include <stdlib.h>"
        "int main()"
        "{"
        "    int *ptr = malloc(sizeof int * 2);"
        "    ptr[0] = 100;"
        "    ptr[1] = 50;"
        "    int sum = ptr[0] + ptr[1];"
        "    free(ptr);"
        "    return sum;"
        "}", 150);
}

TEST(SimpleProgramsTest, subscriptWithCast)
{
    assertExitCodeStdlib(
        "#include <stdlib.h>"
        "int main()"
        "{"
        "    int val = 0;"
        "    char *ptr = (char*)&val;"
        "    ptr[0] = 0x78;"
        "    ptr[1] = 0x56;"
        "    ptr[2] = 0x34;"
        "    ptr[3] = 0x12;"
        "    return val;"
        "}", 0x12345678);
}

TEST(SimpleProgramsTest, subscriptNegativeIndex)
{
    assertExitCodeStdlib(
        "#include <stdlib.h>"
        "int main()"
        "{"
        "    int *ptr = malloc(8);"
        "    int *nptr = ptr + 1;"
        "    nptr[-1] = 15;"
        "    int res = ptr[0];"
        "    free(ptr);"
        "    return res;"
        "}", 15);
}

TEST(SimpleProgramsTest, subscriptEvaluationOrder)
{
    // a[foo()] = foo();
    //    ^        ^
    //   2nd      1st
    assertExitCodeStdlib(
        "#include <stdlib.h>"
        ""
        "int global = 0;"
        "int foo() { return global++; }"
        ""
        "int main()"
        "{"
        "    int *buf = malloc(8);"
        "    buf[0] = 120;"
        "    buf[1] = 120;"
        ""
        "    buf[foo()] = foo();"
        "    int res = buf[1];"
        ""
        "    if (buf[0] != 120)"
        "        return -1;"
        "    if (buf[1] != 0)"
        "        return -2;"
        "    if (global != 2)"
        "        return -3;"
        "    free(buf);"
        "    return 0;"
        "}", 0);
}

TEST(SimpleProgramsTest, leakingMemoryDoesNotCrash)
{
    assertExitCodeStdlib(
        "#include <stdlib.h>"
        "int main() { malloc(40); return 0; }",
        0
    );
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

TEST(SimpleProgramsTest, cannotAssignVoidFunctionToVariable)
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

TEST(SimpleProgramsTest, cannotTakeAddressOfLiterals)
{
    assertCompilationFailure(
        "void main() {"
        "   int *ptr = &100;"
        "}"
    );
}

TEST(SimpleProgramsTest, CONSIDER_changingConstThroughPointer)
{
    assertCompilationFailure(
        "void main() {"
        "   const int var = 5;"
        "   int *ptr = &var;"
        "   *ptr = 6;"
        "   return var;"
        "}"
    );
}

TEST(SimpleProgramsTest, cannotAssignStringLiteralToMutableCharPtr)
{
    assertCompilationFailure(
        "void main() {"
        "   char *str = \"abc\";"
        "}"
    );
}

TEST(SimpleProgramsTest, cannotAssignMutablePtrWithConstPtr)
{
    assertCompilationFailure(
        "void main() {"
        "   int val = 15;"
        "   const int *cptr = &val;"
        "   int *ptr = cptr;"
        "}"
    );
}

TEST(SimpleProgramsTest, cannotPassStringLiteralToCharPtrArgument)
{
    assertCompilationFailure(
        "void foo(char *str) {}"
        "void main() { foo(\"heisann\"); }"
    );
}

TEST(SimpleProgramsTest, includingUndefinedModulesThrows)
{
    assertCompilationFailure(
        "#include <undefined.h>"
        "int main() {return 0;}"
    );
}

static const std::vector<std::string> reservedKeywords = {
    "sizeof", "for", "do", "while", "switch", "case", "goto",
    "bool", "char", "short", "int", "long", "float", 
    "double", "void", "unsigned"
};

TEST(SimpleProgramsTest, reservedKeywordCheckInVariableNames)
{
    for (const std::string &keyword: reservedKeywords) {
        const std::string source = "int main() { int "+keyword+" = 10; return 0; }";
        assertCompilationFailure(source);
    }
}

TEST(SimpleProgramsTest, reservedKeywordCheckInFunctionNames)
{
    for (const std::string &keyword: reservedKeywords) {
        const std::string source = "void "+keyword+"() {} int main() { return 0; }";
        assertCompilationFailure(source);
    }
}

TEST(SimpleProgramsTest, cannotArithmeticallyAssignConstVar)
{
    assertCompilationFailure(
        "int main() {"
        "   const int var = 10;"
        "   var += 10;"
        "   return 0;"
        "}"
    );
}


/* RUNTIME FAILURES */

void assertRuntimeFailure(const std::string &source)
{
    ModuleContext::Ptr context = ModuleContext::create();
    context->addModule(stdlib::buildModule());

    VmPtr vm = createVm(std::move(context), source);
    vm->startSync();
    while (vm->isRunning()) {
        vm->executeNextStatement();
    }

    ASSERT_FALSE(vm->getRuntimeError().get() == nullptr);
}

TEST(SimpleProgramsTest, derefNullPointerFails)
{
    assertRuntimeFailure(
        "int main() {"
        "   int *ptr = 0;"
        "   *ptr;"
        "   return 0;"
        "}"
    );
}

TEST(SimpleProgramsTest, derefAssignmentFromNullPointerFails)
{
    assertRuntimeFailure(
        "int main() {"
        "   int *ptr = 0;"
        "   int val = *ptr;"
        "   return 0;"
        "}"
    );
}

TEST(SimpleProgramsTest, derefUnallocatedLocationThrows)
{
    assertRuntimeFailure(
        "int main() {"
        "   int val = 0;"
        "   int *ptr = &val;"
        "   ptr++; ptr++; ptr++;"
        "   return *ptr;"
        "}"
    );
}

TEST(SimpleProgramsTest, derefDeallocatedLocationThrows)
{
    assertRuntimeFailure(
        "int main() {"
        "   int *ptr = 0;"
        "   if (true) {"
        "       int val = 150;"
        "       ptr = &val;"
        "   }"
        "   *ptr;"
        "   return 0;"
        "}"
    );
}

TEST(SimpleProgramsTest, divisionByZero)
{
    assertRuntimeFailure("int main() { return 10 / 0; }");
    assertRuntimeFailure("int main() { return 10.f / 0.f; }");
}

TEST(SimpleProgramsTest, moduloByZero)
{
    assertRuntimeFailure("int main() { return 10 % 0; }");
}

TEST(SimpleProgramsTest, crashInducedMemoryLeakDoesNotFatallyCrash)
{
    assertRuntimeFailure(
        "#include <stdlib.h>"
        "int main() { malloc(40); return 10 / 0; }"
    );
}

