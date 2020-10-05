#include "vm/VirtualMachine.h"
#include "vm/ExecutionContext.h"

#include "ast/AstBuilder.h"
#include "ast/Ast.h"
#include "ast/AntlrContext.h"

#include "module/stdio/stdioModule.h"
#include "module/stdlib/stdlibModule.h"
#include "module/string/stringModule.h"

#include <iostream>
#include <functional>
#include <unistd.h>
#include <vector>

struct CliArgs {
    uint32_t memorySize;
    uint32_t allocationSize;
    std::string fileName;

    // Command line arguments to pass to the VM
    std::vector<std::string> args;
};

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

cish::module::ModuleContext::Ptr createModuleContext()
{
    cish::module::ModuleContext::Ptr moduleContext = cish::module::ModuleContext::create();
    moduleContext->addModule(cish::module::stdlib::buildModule());
    moduleContext->addModule(cish::module::stdio::buildModule());
    moduleContext->addModule(cish::module::string::buildModule());

    // Bools are natively handled in cish, so add an empty module for compat
    cish::module::Module::Ptr stdbool = cish::module::Module::create("stdbool.h");
    moduleContext->addModule(stdbool);

    return moduleContext;
}

int execute(CliArgs args)
{
    std::ifstream t(args.fileName);
    std::string source((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    cish::module::ModuleContext::Ptr moduleContext = createModuleContext();

    cish::ast::ParseContext::Ptr parseContext = cish::ast::ParseContext::parseSource(source);
    cish::ast::AstBuilder builder(parseContext, std::move(moduleContext));

    cish::ast::Ast::Ptr ast;

    if (!doTry([&]() {ast = builder.buildAst();})) {
        return 1;
    }

    cish::vm::VmOptions opts;
    opts.heapSize = args.memorySize;
    opts.minAllocSize = 4;
    opts.args.push_back(args.fileName);
    for (auto a: args.args) {
        opts.args.push_back(a);
    }

    cish::vm::VirtualMachine vm(opts, std::move(ast));
    vm.executeBlocking();

    auto err = vm.getRuntimeError();
    if (err != nullptr) {
        std::cerr << err->userMessage() << std::endl;
        return 1;
    }

    fflush(stdout);
    fflush(stderr);

    return vm.getExitCode();
}


// -----

int parseIntArg(char option, const char *str)
{
    int value = atoi(str);
    if (value == 0) {
        fprintf(stderr, "Argument '%s' is not valid for option '%c'", str, option);
        exit(1);
    }

    return value;
}

int main(int argc, char **argv)
{
    bool haltAfterExec = false;

    CliArgs args;
    args.allocationSize = 4;
    args.memorySize = 1 << 10;

    int c;
    while ((c = getopt (argc, argv, "ha:m:")) != -1) {
        switch (c) {
            case 'a':
                args.allocationSize = parseIntArg(optopt, optarg);
                fprintf(stderr, "[custom allocation size] %d\n", args.allocationSize);
                break;
            case 'm':
                args.memorySize = parseIntArg(optopt, optarg);
                fprintf(stderr, "[custom memory size] %d\n", args.memorySize);
                break;
            case 'h':
                haltAfterExec = true;
                break;
            case '?':
                if (isalpha(optopt))
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    if (optind == argc) {
        fprintf(stderr, "Missing source file to execute\n");
        exit(1);
    }

    args.fileName = argv[optind];

    for (int i=optind+1; i<argc; i++) {
        args.args.push_back(argv[i]);
    }

    int retval = execute(args);

    if (haltAfterExec) {
        getchar();
    }

    return retval;
}
