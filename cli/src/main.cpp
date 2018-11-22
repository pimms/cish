#include "vm/VirtualMachine.h"
#include "vm/ExecutionContext.h"

#include "ast/AstBuilder.h"
#include "ast/Ast.h"
#include "ast/AntlrContext.h"

#include "module/stdio/stdioModule.h"
#include "module/stdlib/stdlibModule.h"

#include <iostream>
#include <functional>

bool doTry(std::function<void(void)> f) {
    try {
        f();
        return true;
    } catch (cish::Exception e) {
        std::cerr << e.userMessage() << std::endl;
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown failure" << std::endl;
    }
    return false;
}

int execute(int argc, char **argv)
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
    cish::ast::AstBuilder builder(&antlrContext, std::move(moduleContext));

    cish::ast::Ast::Ptr ast;

    if (!doTry([&]() {ast = builder.buildAst();})) {
        return 1;
    }

    cish::vm::VmOptions opts;
    opts.heapSize = 1 << 10;
    opts.minAllocSize = 4;

    cish::vm::VirtualMachine vm(opts, std::move(ast));
    vm.executeBlocking();

    auto err = vm.getRuntimeError();
    if (err != nullptr) {
        std::cerr << err->userMessage() << std::endl;
        return 1;
    }

    return vm.getExitCode();
}

int main(int argc, char **argv)
{
    int retval = execute(argc, argv);

    #ifdef DEBUG
    // Sleep so MacOS Instruments is able to scrape through our
    // memory space.
    //
    // TODO: Convert this into a command-line option
    getchar();
    #endif

    return retval;
}
