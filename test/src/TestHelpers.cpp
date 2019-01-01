#include "TestHelpers.h"
#include "module/ModuleContext.h"
#include "module/stdlib/stdlibModule.h"
#include "module/stdio/stdioModule.h"

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
    moduleContext->addModule(stdlib::buildModule());
    moduleContext->addModule(stdio::buildModule());

    VmPtr vm = createVm(std::move(moduleContext), source);
    vm->startSync();

    while (vm->isRunning()) {
        vm->executeNextStatement();
    }

    EXPECT_EQ(expectedExitCode, vm->getExitCode());
}

void assertCompilationFailure(const std::string &source)
{
    ASSERT_ANY_THROW(createAst(source));
}

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

