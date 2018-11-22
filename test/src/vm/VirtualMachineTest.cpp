#include <gtest/gtest.h>

#include "vm/VirtualMachine.h"
#include "ast/AntlrContext.h"
#include "ast/AstBuilder.h"
#include "ast/LiteralExpression.h"
#include "ast/Ast.h"
#include "vm/Variable.h"
#include "vm/ExecutionContext.h"

#include "../TestHelpers.h"


using namespace cish::vm;
using namespace cish::ast;


TEST(VirtualMachineTest, globalStatementsAreExecutedOneByOne)
{
    VmPtr vm = createVm("int a = 15;  int b = 15 + a; int c; void main(){}");
    vm->startSync();
    vm->executeNextStatement();
    ASSERT_EQ(nullptr, getVar(vm, "a"));
    ASSERT_EQ(nullptr, getVar(vm, "b"));
    ASSERT_EQ(nullptr, getVar(vm, "c"));

    vm->executeNextStatement();
    ASSERT_EQ(15, getVar(vm, "a")->getAllocation()->read<int>());
    ASSERT_EQ(nullptr, getVar(vm, "b"));
    ASSERT_EQ(nullptr, getVar(vm, "c"));

    vm->executeNextStatement();
    ASSERT_EQ(15, getVar(vm, "a")->getAllocation()->read<int>());
    ASSERT_EQ(30, getVar(vm, "b")->getAllocation()->read<int>());
    ASSERT_EQ(nullptr, getVar(vm, "c"));

    vm->executeNextStatement();
    ASSERT_EQ(15, getVar(vm, "a")->getAllocation()->read<int>());
    ASSERT_EQ(30, getVar(vm, "b")->getAllocation()->read<int>());
    ASSERT_NE(nullptr, getVar(vm, "c"));
}

TEST(VirtualMachineTest, executingNextStatementWhenStoppedThrows)
{
    VmPtr vm = createVm("int a = 15;  int b = 15 + a; int c; void main(){}");
    vm->startSync();
    vm->executeNextStatement();
    vm->terminate();

    ASSERT_THROW(vm->executeNextStatement(), VmException);
}

TEST(VirtualMachineTest, terminationIdempotence)
{
    VmPtr vm = createVm("int a = 15;  int b = 15 + a; int c; void main(){}");
    vm->startSync();
    vm->executeNextStatement();
    vm->terminate();
    ASSERT_NO_THROW(vm->terminate());
}

TEST(VirtualMachineTest, terminatingVmHaltsExecution)
{
    VmPtr vm = createVm("int a = 15;  int b = 15 + a; int c; void main(){}");
    vm->startSync();
    vm->executeNextStatement();
    ASSERT_EQ(nullptr, getVar(vm, "a"));
    ASSERT_EQ(nullptr, getVar(vm, "b"));
    ASSERT_EQ(nullptr, getVar(vm, "c"));

    vm->executeNextStatement();
    ASSERT_EQ(15, getVar(vm, "a")->getAllocation()->read<int>());
    ASSERT_EQ(nullptr, getVar(vm, "b"));
    ASSERT_EQ(nullptr, getVar(vm, "c"));

    vm->terminate();

    ASSERT_EQ(15, getVar(vm, "a")->getAllocation()->read<int>());
    ASSERT_EQ(nullptr, getVar(vm, "b"));
    ASSERT_EQ(nullptr, getVar(vm, "c"));
}


TEST(VirtualMachineTest, isRunningReturnsFalsePostTermination)
{
    VmPtr vm = createVm("int a = 15; void main() {}");
    vm->startSync();

    ASSERT_TRUE(vm->isRunning());
    vm->executeNextStatement();
    vm->executeNextStatement();
    vm->executeNextStatement();
    ASSERT_FALSE(vm->isRunning());
}

TEST(VirtualMachineTest, executingBeyondLastStatementThrows)
{
    VmPtr vm = createVm("int a = 15; void main() {}");
    vm->startSync();
    vm->executeNextStatement();
    vm->executeNextStatement();
    vm->executeNextStatement();
    ASSERT_FALSE(vm->isRunning());
    ASSERT_THROW(vm->executeNextStatement(), VmException);
}

TEST(VirtualMachineTest, gettingExitCodeThrowsIfVmIsRunning)
{
    VmPtr vm = createVm("void main() {}");
    vm->startSync();

    ASSERT_TRUE(vm->isRunning());
    ASSERT_ANY_THROW(vm->getExitCode());
}

TEST(VirtualMachineTest, startPostSynchronousThrows)
{
    VmPtr vm = createVm(
        "int main() { int var = 10; return var; }");

    vm->startSync();
    while (vm->isRunning())
        vm->executeNextStatement();
    ASSERT_EQ(10, vm->getExitCode());

    ASSERT_THROW(vm->startSync(), VmException);
    ASSERT_THROW(vm->executeBlocking(), VmException);
}

TEST(VirtualMachineTest, startPostTerminationThrows)
{
    VmPtr vm = createVm(
        "int main() { int var = 10; return var; }");

    vm->startSync();
    vm->executeNextStatement();
    vm->executeNextStatement();
    vm->terminate();

    ASSERT_THROW(vm->startSync(), VmException);
    ASSERT_THROW(vm->executeBlocking(), VmException);
}

TEST(VirtualMachineTest, startPostBlockingThrows)
{
    VmPtr vm = createVm(
        "int main() { int var = 10; return var; }");

    vm->executeBlocking();
    ASSERT_EQ(10, vm->getExitCode());

    ASSERT_THROW(vm->startSync(), VmException);
    ASSERT_THROW(vm->executeBlocking(), VmException);
}

