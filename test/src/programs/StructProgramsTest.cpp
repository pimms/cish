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


TEST(StructProgramsTest, obj_assignAndReturnField)
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

TEST(StructProgramsTest, obj_assignAndMultiplyFields)
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

TEST(SructProgramsTest, obj_passFieldToFunction)
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

TEST(StructProgramsTest, obj_combinedFieldOps)
{
    assertExitCode(
        "struct node_t {"
        "    int val1;"
        "    float val2;"
        "};"
        "int echo(int n) { return n; }"
        "int main() {"
        "    struct node_t var;"
        "    var.val1 = 10;"
        "    var.val2 = 4.9f;"
        "    int param = var.val1 * var.val2;"
        "    return echo(var.val1 * var.val2);"
        "}", 49
    );
}


TEST(StructProgramsTest, ptr_assignAndReturnField)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct node_t {"
        "   int value;"
        "};"
        "int main() {"
        "   struct node_t *var = malloc(sizeof(struct node_t));"
        "   var->value = 20;"
        "   return var->value + 4;"
        "}", 24
    );
}

TEST(StructProgramsTest, ptr_assignAndMultiplyFields)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct node_t {"
        "   int val1;"
        "   float val2;"
        "};"
        "int main() {"
        "   struct node_t *var = malloc(sizeof(struct node_t));"
        "   var->val1 = 10;"
        "   var->val2 = 4.9;"
        "   return var->val1 * var->val2;"
        "}", 49
    );
}

TEST(SructProgramsTest, ptr_passFieldToFunction)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct node_t {"
        "   float val;"
        "};"
        "int echo(int n) { return n; }"
        "int main() {"
        "   struct node_t *var = malloc(sizeof(struct node_t));"
        "   var->val = (int)199;"
        "   return (int)echo((float)var->val);"
        "}", 199
    );
}

TEST(StructProgramsTest, ptr_combinedFieldOps)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct node_t {"
        "    int val1;"
        "    float val2;"
        "};"
        "int echo(int n) { return n; }"
        "int main() {"
        "    struct node_t *var = malloc(sizeof(struct node_t));"
        "    var->val1 = 10;"
        "    var->val2 = 4.9f;"
        "    return echo(var->val1 * var->val2);"
        "}", 49
    );
}

TEST(StructProgramsTest, ptr_recursiveStruct)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct node_t { struct node_t *next; int val; }; "
        "int main() {"
        "   struct node_t root;"
        "   root.val = 1;"
        "   root.next = malloc(sizeof(struct node_t));"
        "   root.next->val = 2;"
        "   root.next->next = malloc(sizeof(struct node_t));"
        "   root.next->next->val = 3;"
        "   int res = root.val + root.next->val + root.next->next->val;"
        "   free(root.next->next);"
        "   free(root.next);"
        "   return res;"
        "}", 1 + 2 + 3
    );
}


TEST(StructProgramsTest, obj_assignmentOfStruct)
{
    assertExitCode(
        "struct item { int a; int b; };"
        "int main() {"
        "   struct item a;"
        "   a.a = 5;"
        "   a.b = 10;"
        ""
        "   struct item b = a;"
        "   b.a += 5;"
        "   b.b += 5;"
        ""
        "   struct item c;"
        "   c = b;"
        "   c.a = 1;"
        ""
        "   return a.a+a.b + b.a+b.b + c.a+c.b;"
        "}", 5 + 10 + 10 + 15 + 1 + 15
    );
}

TEST(StructProgramsTest, obj_assignmentFromDerefPointer)
{
    assertExitCode(
        "#include <stdlib.h>"
        "struct item { int a; int b; };"
        "int main() {"
        "   struct item *orig = malloc(sizeof(struct item));"
        "   orig->a = 10;"
        "   orig->b = 10;"
        ""
        "   struct item copy = *orig;"
        "   orig->a++;"
        "   orig->b++;"
        ""
        "   /* leak schmeak */"
        "   return orig->a + orig->b + copy.a + copy.b;"
        "}", 11 + 11 + 10 + 10
    );
}


TEST(StructProgramsTest, passByValue)
{
    assertExitCode(
        "struct item { int a; int b; };"
        "int double_sum(struct item item) {"
        "   item.a *= 2;"
        "   item.b *= 2;"
        "   return item.a + item.b;"
        "}"
        "int main() {"
        "   struct item item;"
        "   item.a = 10;"
        "   item.b = 15;"
        ""
        "   int n = double_sum(item);"
        "   return item.a + item.b + n;"
        "}", 10 + 15 + 20 + 30
    );
}


TEST(StructProgramsTest, returnStructByValue)
{
    assertExitCode(
        "struct item_t { int a; int b; };"
        "struct item_t foo() {"
        "   struct item_t item;"
        "   item.a = 9;"
        "   item.b = 11;"
        "   if (1 == 1) {"
        "       return item;"
        "   }"
        "}"
        "int main() {"
        "   struct item_t item = foo();"
        "   return item.a + item.b;"
        "}", 20
    );
}

TEST(StructProgramsTest, recursiveStructByValue)
{
    assertExitCode(
        "struct wrapper_t { int value; int count; };"
        "struct wrapper_t foo(struct wrapper_t wrapper) {"
        "   if (wrapper.value >= 100) {"
        "       return wrapper;"
        "   } else {"
        "       wrapper.value += 5;"
        "       wrapper.count++;"
        "       return foo(wrapper);"
        "   }"
        "}"
        "int main() {"
        "   struct wrapper_t wrap;"
        "   wrap.value = 0;"
        "   wrap.count = 0;"
        "   wrap = foo(wrap);"
        "   return wrap.value * wrap.count;"
        "}", 100 * 20
    );
}

TEST(StructProgramsTest, passStructFuncAsParam)
{
    assertExitCode(
        "struct item_t { int n; };"
        ""
        "int get_n(struct item_t a, struct item_t b) { "
        "   return a.n + b.n;"
        "}"
        ""
        "int global = 99;"
        "struct item_t get_item() {"
        "   struct item_t item;"
        "   item.n = global++;"
        "   return item;"
        "}"
        "int main() {"
        "   return get_n(get_item(), get_item());"
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

TEST(StructProgramsTest, structByValueAreNotImplicitlyConvertible)
{
    assertCompilationFailure(
        "struct A { int n; };"
        "struct B { int n; };"
        "int main() {"
        "   struct A a;"
        "   struct B b = a;"
        "   return 0;"
        "}"
    );
}



/* RUNTIME FAILURES */
