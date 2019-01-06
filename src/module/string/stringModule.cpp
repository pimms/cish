#include "stringModule.h"
#include "../../vm/ExecutionContext.h"


namespace cish
{

using namespace ast;

namespace module
{
namespace string
{


Module::Ptr buildModule()
{
    Module::Ptr module = Module::create("string.h");
    module->addFunction(std::make_shared<impl::Memchr>());
    module->addFunction(std::make_shared<impl::Memcmp>());
    module->addFunction(std::make_shared<impl::Memcpy>());
    return module;
}


namespace impl
{


/*
==================
void *memchr(const void *str, int c, int n)
==================
*/
ast::FuncDeclaration Memchr::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::VOID)),
        "memchr",
        {
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::VOID)),
                "str"
            },
            {
                TypeDecl::CHAR,
                "c"
            },
            {
                TypeDecl::INT,
                "n"
            }
        }
    );
}

Memchr::Memchr(): Function(getSignature()) {}

ExpressionValue Memchr::execute(vm::ExecutionContext *context, FuncParams params, vm::Variable*) const
{
    const uint32_t haystackAddress = params[0].get<uint32_t>();
    const uint8_t needle = params[1].get<uint8_t>();
    const uint32_t maxLength = params[2].get<int32_t>();

    vm::MemoryView buffer = context->getMemory()->getView(haystackAddress);
    const TypeDecl type = TypeDecl::getPointer(TypeDecl::VOID);

    int index = 0;
    bool found = false;
    while (index < maxLength) {
        if (buffer.read<uint8_t>(index) == needle) {
            return ExpressionValue(type, haystackAddress + index);
        }

        index++;
    }

    return ExpressionValue(type, 0);
}


/*
==================
int memcmp(const void *str1, const void *str2, size_t n)
==================
*/
ast::FuncDeclaration Memcmp::getSignature()
{
    return FuncDeclaration(
        TypeDecl::INT,
        "memcmp",
        {
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::VOID)),
                "str1"
            },
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::VOID)),
                "str2"
            },
            {
                TypeDecl::INT,
                "n"
            }
        }
    );
}

Memcmp::Memcmp(): Function(getSignature()) { }

ast::ExpressionValue Memcmp::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t addr1 = params[0].get<uint32_t>();
    const uint32_t addr2 = params[1].get<uint32_t>();
    const int maxLen = params[2].get<int>();

    vm::MemoryView view1 = context->getMemory()->getView(addr1);
    vm::MemoryView view2 = context->getMemory()->getView(addr2);

    int index = 0;
    while (index < maxLen) {
        const uint8_t b1 = view1.read<uint8_t>(index);
        const uint8_t b2 = view2.read<uint8_t>(index);

        if (b1 != b2) {
            return ExpressionValue(TypeDecl::INT, (b1 < b2 ? -1 : 1));
        }

        index++;
    }

    return ExpressionValue(TypeDecl::INT, 0);
}


/*
==================
void *memcpy(void *dest, const void *src, size_t n)
==================
*/
ast::FuncDeclaration Memcpy::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::VOID),
        "memcpy",
        {
            {
                TypeDecl::getPointer(TypeDecl::VOID),
                "dest"
            },
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::VOID)),
                "src",
            },
            {
                TypeDecl::INT,
                "n"
            }
        }
    );
}

Memcpy::Memcpy(): Function(getSignature()) { }

ast::ExpressionValue Memcpy::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t destAddr = params[0].get<uint32_t>();
    const uint32_t srcAddr = params[1].get<uint32_t>();
    const int length = params[2].get<int>();

    vm::MemoryView dest = context->getMemory()->getView(destAddr);
    vm::MemoryView src = context->getMemory()->getView(srcAddr);

    for (int i=0; i<length; i++) {
        dest.write(src.read<uint8_t>(i), i);
    }

    return ExpressionValue(TypeDecl::getPointer(TypeDecl::VOID), destAddr);
}



}

}
}
}
