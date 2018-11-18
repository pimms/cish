#include "vm/VirtualMachine.h"
#include "vm/ExecutionContext.h"

#include "ast/AstBuilder.h"
#include "ast/Ast.h"
#include "ast/AntlrContext.h"

#include "module/stdio/stdioModule.h"
#include "module/stdlib/stdlibModule.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file.c>\n", argv[0]);
        return 1;
    }

    std::ifstream t(argv[1]);
    std::string source((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    cish::module::ModuleContext::Ptr moduleContext = cish::module::ModuleContext::create();
    moduleContext->addModule(cish::module::stdlib::buildModule());
    moduleContext->addModule(cish::module::stdio::buildModule());

    cish::ast::AntlrContext antlrContext(source);
    cish::ast::AstBuilder builder(&antlrContext, moduleContext);
    cish::ast::Ast::Ptr ast = builder.buildAst();

    cish::vm::VmOptions opts;
    opts.heapSize = 1 << 10;
    opts.minAllocSize = 4;

    cish::vm::VirtualMachine vm(opts, std::move(ast));
    vm.executeBlocking();
    return vm.getExitCode();
}
