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



}

}
}
}
