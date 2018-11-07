#include <gtest/gtest.h>

#include "vm/VirtualMachine.h"
#include "ast/AntlrContext.h"
#include "ast/AstBuilder.h"
#include "ast/LiteralExpression.h"
#include "ast/Ast.h"
#include "vm/Variable.h"
#include "vm/ExecutionContext.h"


using namespace cish::vm;
using namespace cish::ast;


VirtualMachine* createVm(const std::string &source)
{
    AntlrContext *antlrContext = new AntlrContext(source);
    AstBuilder builder(antlrContext);
    Ast::Ptr ast = builder.buildAst();

    VmOptions opts;
    opts.heapSize = 512;
    opts.minAllocSize = 4;
    return new VirtualMachine(opts, std::move(ast));
}

Variable* getVar(VirtualMachine *vm, const std::string &name)
{
    return vm->getExecutionContext()->getStackFrame()->getVariable(name);
}

TEST(VirtualMachineTest, globalStatementsAreExecutedOneByOne)
{
    VirtualMachine *vm = createVm("int a = 15;  int b = 15 + a; int c; void main(){}");
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

    delete vm;
}
