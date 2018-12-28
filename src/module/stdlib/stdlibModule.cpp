#include "stdlibModule.h"

#include "../Utils.h"
#include "../../ast/Type.h"
#include "../../vm/Allocation.h"
#include "../../vm/ExecutionContext.h"
#include "../../vm/Memory.h"


namespace cish
{
namespace module
{
namespace stdlib
{


Module::Ptr buildModule()
{
    MallocContext::Ptr mallocContext = MallocContext::Ptr(new MallocContext());

    Module::Ptr module = Module::create("stdlib.h");
    module->addFunction(Function::Ptr(new impl::Atof()));
    module->addFunction(Function::Ptr(new impl::Atoi()));
    module->addFunction(Function::Ptr(new impl::Rand()));
    module->addFunction(Function::Ptr(new impl::Srand()));
    module->addFunction(Function::Ptr(new impl::Malloc(mallocContext)));
    module->addFunction(Function::Ptr(new impl::Free(mallocContext)));

    return module;
}



namespace impl
{

using ast::TypeDecl;
using ast::FuncDeclaration;
using ast::VarDeclaration;
using ast::ExpressionValue;
using vm::MemoryView;
using vm::Allocation;


/*
==================
double atof(const char *s)
==================
*/
ast::FuncDeclaration Atof::getSignature()
{
    FuncDeclaration decl;
    decl.returnType = TypeDecl::DOUBLE;
    decl.name = "atof";

    decl.params.push_back(
        VarDeclaration {
            TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
            "s"
        }
    );

    return decl;
}

Atof::Atof(): Function(getSignature()) {}

ast::ExpressionValue Atof::execute(vm::ExecutionContext *context,
                                   FuncParams params) const
{
    ExpressionValue param = params[0];
    const uint32_t addr = param.get<uint32_t>();
    MemoryView view = context->getMemory()->getView(addr);

    std::vector<char> str;
    utils::readString(view, str);

    const double result = atof(str.data());
    return ExpressionValue(TypeDecl::DOUBLE, result);
}


/*
==================
int atoi(const char *s)
==================
*/
ast::FuncDeclaration Atoi::getSignature()
{
    FuncDeclaration decl;
    decl.returnType = TypeDecl::INT;
    decl.name = "atoi";

    decl.params.push_back(
        VarDeclaration {
            TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
            "s"
        }
    );

    return decl;
}

Atoi::Atoi(): Function(getSignature()) {}

ast::ExpressionValue Atoi::execute(vm::ExecutionContext *context,
                                   FuncParams params) const
{
    ExpressionValue param = params[0];
    const uint32_t addr = param.get<uint32_t>();
    MemoryView view = context->getMemory()->getView(addr);

    std::vector<char> str;
    utils::readString(view, str);

    const int result = atoi(str.data());
    return ExpressionValue(TypeDecl::INT, result);
}


/*
==================
int rand()
==================
*/
ast::FuncDeclaration Rand::getSignature()
{
    return FuncDeclaration(TypeDecl::INT, "rand");
}

Rand::Rand(): Function(getSignature()) {}

ast::ExpressionValue Rand::execute(vm::ExecutionContext *context,
                                   FuncParams params) const
{
    return ExpressionValue(TypeDecl::INT, (int)rand());
}


/*
==================
void srand(int seed)
==================
*/
ast::FuncDeclaration Srand::getSignature()
{
    return FuncDeclaration(
        TypeDecl::VOID,
        "srand",
        {VarDeclaration { TypeDecl::INT, "seed" }}
    );
}

Srand::Srand(): Function(getSignature()) {}

ast::ExpressionValue Srand::execute(vm::ExecutionContext *context,
                                    FuncParams params) const
{
    srand(params[0].get<int>());
    return ExpressionValue(TypeDecl::VOID);
}


/*
==================
void* malloc(int)
==================
*/
ast::FuncDeclaration Malloc::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::VOID),
        "malloc",
        {VarDeclaration { TypeDecl::INT, "size" }}
    );
}

Malloc::Malloc(MallocContext::Ptr mallocContext):
    Function(getSignature()),
    _mallocContext(mallocContext)
{}

ast::ExpressionValue Malloc::execute(vm::ExecutionContext *context,
                                     FuncParams params) const
{
    try {
        const uint32_t size = params[0].get<uint32_t>();
        Allocation::Ptr alloc = context->getMemory()->allocate(size);

        const uint32_t addr = alloc->getAddress();
        _mallocContext->onAllocation(std::move(alloc));

        return ExpressionValue(TypeDecl::getPointer(TypeDecl::VOID), addr);
    } catch (const vm::OutOfMemoryException &e) {
        return ExpressionValue(TypeDecl::getPointer(TypeDecl::VOID), 0);
    }
}


/*
==================
void free(void *p)
==================
*/
ast::FuncDeclaration Free::getSignature()
{
    return FuncDeclaration(
        TypeDecl::VOID,
        "free",
        {VarDeclaration { TypeDecl::getPointer(TypeDecl::VOID), "ptr" }}
    );
}

Free::Free(MallocContext::Ptr mallocContext):
    Function(getSignature()),
    _mallocContext(mallocContext)
{}

ast::ExpressionValue Free::execute(vm::ExecutionContext *context,
                                     FuncParams params) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    if (!_mallocContext->attemptDeallocation(addr)) {
        Throw(StdlibException,
            "buffer %x was not allocated with malloc or has already been freed",
            addr);
    }

    return ExpressionValue(TypeDecl::VOID);
}


}
}
}
}
