#include <gtest/gtest.h>

#include "../TestHelpers.h"
#include "vm/VirtualMachine.h"
#include "module/stdlib/stdlibModule.h"

using namespace cish::vm;
using namespace cish::ast;
using namespace cish::module;



/* VALID PROGRAMS */

TEST(StructProgramsTest, structDeclaration)
{
    assertExitCode(
        "struct s { int n; };"
        "int main() {"
        "    return 0;"
        "}", 0
    );
}

TEST(StructProgramsTest, variableDeclaration)
{
    assertExitCode(
        "struct node_t{ int n; };"
        "int main() {"
        "   struct node_t var;"
        "   return 0;"
        "}", 0
    );
}

TEST(StructProgramsTest, sizeofStruct)
{
    assertExitCode(
        "struct node_t{"
        "   struct node_t *left;"
        "   struct node_t *right;"
        "   int value;"
        "};"
        "int main() {"
        "   struct node_t var;"
        "   return sizeof var + sizeof struct node_t;"
        "}", 24
    );
}

TEST(StructProgramsTest, assignAndReturnField)
{
    assertExitCode(
        "struct node_t {"
        "   int value;"
        "};"
        "int main() {"
        "   struct node_t var;"
        "   var.value = 20;"
        "   return var.value + 4;"
        "}", 24
    );
}

TEST(StructProgramsTest, assignAndMultiplyFields)
{
    assertExitCode(
        "struct node_t {"
        "   int val1;"
        "   float val2;"
        "};"
        "int main() {"
        "   struct node_t var;"
        "   var.val1 = 10;"
        "   var.val2 = 4.9;"
        "   return var.val1 * var.val2;"
        "}", 49
    );
}

TEST(SructProgramsTest, passFieldToFunction)
{
    assertExitCode(
        "struct node_t {"
        "   int val;"
        "};"
        "int echo(int n) { return n; }"
        "int main() {"
        "   struct node_t var;"
        "   var.val = 199;"
        "   return echo(var.val);"
        "}", 199
    );
}


/* COMPILATION FAILURES */

TEST(StructProgramsTest, emptyStructsAreNotAllowed)
{
    assertCompilationFailure("struct foo {}; int main(){ return 0; }");
}

TEST(StructProgramsTest, redeclaringStructsIsNotAllowed)
{
    assertCompilationFailure(
        "struct foo { int a; };"
        "struct foo { int b; };"
        "int main(){ return 0; }"
    );
}

TEST(StructProgramsTest, defaultValueForStructsIsNotAllowed)
{
    assertCompilationFailure(
        "struct foo { int a = 10; };"
        "int main(){ return 0; }"
    );
}

TEST(StructProgramsTest, structSemicolonRequirements)
{
    assertCompilationFailure(
        "struct foo { int a };"
        "int main(){ return 0; }"
    );
    assertCompilationFailure(
        "struct foo { int a; }"
        "int main(){ return 0; }"
    );
}

TEST(StructProgramsTest, structsMustBeDeclaredBeforeUse)
{
    assertCompilationFailure(
        "struct somethingElse {int n;};"
        "int main() {"
        "   struct foo var;"
        "   return 0;"
        "}"
    );
}

TEST(StructProgramsTest, recursiveNonPointerStructs)
{
    assertCompilationFailure(
        "struct t { struct t val;};"
        "int main() {"
        "   return 0;"
        "}"
    );
}



/* RUNTIME FAILURES */
