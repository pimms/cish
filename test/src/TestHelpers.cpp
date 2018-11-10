#include "TestHelpers.h"


VmPtr createVm(const std::string &source)
{
    cish::ast::AntlrContext antlrContext(source);
    cish::ast::AstBuilder builder(&antlrContext);
    cish::ast::Ast::Ptr ast = builder.buildAst();

    cish::vm::VmOptions opts;
    opts.heapSize = 512;
    opts.minAllocSize = 4;
    return VmPtr(new cish::vm::VirtualMachine(opts, std::move(ast)));
}

cish::vm::Variable* getVar(VmPtr vm, const std::string &name)
{
    return vm->getExecutionContext()->getScope()->getVariable(name);
}

