#include "TestHelpers.h"
#include "module/ModuleContext.h"

using namespace cish::vm;
using namespace cish::ast;
using namespace cish::module;


Ast::Ptr createAst(const std::string &source)
{
    return createAst(ModuleContext::create(), source);
}

Ast::Ptr createAst(ModuleContext::Ptr moduleContext, const std::string &source)
{

    ParseContext::Ptr parseContext = ParseContext::parseSource(source);

    AstBuilder builder(parseContext, std::move(moduleContext));
    Ast::Ptr ast = builder.buildAst();
    return ast;
}

VmPtr createVm(const std::string &source)
{
    return createVm(ModuleContext::create(), source);
}

VmPtr createVm(ModuleContext::Ptr moduleContext, const std::string &source)
{
    Ast::Ptr ast = createAst(std::move(moduleContext), source);

    VmOptions opts;
    opts.heapSize = 512;
    opts.minAllocSize = 4;
    return VmPtr(new VirtualMachine(opts, std::move(ast)));
}

Variable* getVar(VmPtr vm, const std::string &name)
{
    return vm->getExecutionContext()->getScope()->getVariable(name);
}



void assertExitCode(const std::string &source, int expectedExitCode)
{
    assertExitCode(ModuleContext::create(), source, expectedExitCode);
}

void assertExitCode(ModuleContext::Ptr moduleContext, const std::string &source, int expectedExitCode)
{
    VmPtr vm = createVm(std::move(moduleContext), source);
    vm->startSync();

    while (vm->isRunning()) {
        vm->executeNextStatement();
    }

    EXPECT_EQ(expectedExitCode, vm->getExitCode());
}
