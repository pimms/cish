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
    module->addFunction(std::make_shared<impl::Memset>());
    module->addFunction(std::make_shared<impl::Strcat>());
    module->addFunction(std::make_shared<impl::Strncat>());
    module->addFunction(std::make_shared<impl::Strchr>());
    module->addFunction(std::make_shared<impl::Strcmp>());
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


/*
==================
void *memset(void *str, int c, size_t n)
==================
*/
ast::FuncDeclaration Memset::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::VOID),
        "memset",
        {
            {
                TypeDecl::getPointer(TypeDecl::VOID),
                "str"
            },
            {
                TypeDecl::INT,
                "c",
            },
            {
                TypeDecl::INT,
                "n"
            }
        }
    );
}

Memset::Memset(): Function(getSignature()) { }

ast::ExpressionValue Memset::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    const uint8_t character = params[1].get<uint8_t>();
    const int length = params[2].get<int>();

    vm::MemoryView view = context->getMemory()->getView(addr);

    for (int i=0; i<length; i++) {
        view.write<uint8_t>(character, i);
    }

    return ExpressionValue(TypeDecl::getPointer(TypeDecl::VOID), addr);
}


/*
==================
char *strcat(char *dest, const char *src)
==================
*/
ast::FuncDeclaration Strcat::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::CHAR),
        "strcat",
        {
            {
                TypeDecl::getPointer(TypeDecl::CHAR),
                "dest"
            },
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "src"
            }
        }
    );
}

Strcat::Strcat(): Function(getSignature()) { }

ast::ExpressionValue Strcat::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t destAddr = params[0].get<uint32_t>();
    const uint32_t srcAddr = params[1].get<uint32_t>();

    vm::MemoryView destView = context->getMemory()->getView(destAddr);
    vm::MemoryView srcView = context->getMemory()->getView(srcAddr);

    uint32_t offset = 0;
    while (destView.read<uint8_t>(offset) != 0) {
        offset++;
    }

    uint32_t i = 0;
    uint8_t ch = 0;
    do {
        ch = srcView.read<uint8_t>(i);
        destView.write<uint8_t>(ch, offset + i);
        i += 1;
    } while (ch);

    return ExpressionValue(TypeDecl::getPointer(TypeDecl::CHAR), destAddr);
}


/*
==================
char *strncat(char *dest, const char *src, size_t n)
==================
*/
ast::FuncDeclaration Strncat::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::CHAR),
        "strncat",
        {
            {
                TypeDecl::getPointer(TypeDecl::CHAR),
                "dest"
            },
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "src"
            },
            {
                TypeDecl::INT,
                "n"
            }
        }
    );
}

Strncat::Strncat(): Function(getSignature()) { }

ast::ExpressionValue Strncat::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t destAddr = params[0].get<uint32_t>();
    const uint32_t srcAddr = params[1].get<uint32_t>();
    const uint32_t maxLen = params[2].get<uint32_t>();

    vm::MemoryView destView = context->getMemory()->getView(destAddr);
    vm::MemoryView srcView = context->getMemory()->getView(srcAddr);

    uint32_t offset = 0;
    while (destView.read<uint8_t>(offset) != 0) {
        offset++;
    }

    uint32_t i = 0;
    uint8_t ch = 0;

    do {
        ch = srcView.read<uint8_t>(i);
        destView.write<uint8_t>(ch, offset + i);
        i += 1;
    } while (ch && i < maxLen);

    destView.write<uint8_t>(0, offset + i);

    return ExpressionValue(TypeDecl::getPointer(TypeDecl::CHAR), destAddr);
}


/*
==================
char *strchr(const char *str, int character)
==================
*/
ast::FuncDeclaration Strchr::getSignature()
{
    return FuncDeclaration(
        TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
        "strchr",
        {
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "str"
            },
            {
                TypeDecl::INT,
                "character"
            }
        }
    );
}

Strchr::Strchr(): Function(getSignature()) { }

ast::ExpressionValue Strchr::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    const uint8_t needle = params[1].get<uint8_t>();

    vm::MemoryView view = context->getMemory()->getView(addr);

    uint32_t result = 0;
    uint32_t offset = 0;
    while (true) {
        const uint8_t ch = view.read<uint8_t>(offset);
        if (ch == needle) {
            result = addr + offset;
            break;
        } else if (ch == 0) {
            result = 0;
            break;
        }

        offset++;
    }

    return ExpressionValue(TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)), result);
}


/*
==================
int strcmp(const char *str1, const char *str2)
==================
*/
ast::FuncDeclaration Strcmp::getSignature()
{
    return FuncDeclaration(
        TypeDecl::INT,
        "strcmp",
        {
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "str1"
            },
            {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "str2"
            }
        }
    );
}

Strcmp::Strcmp(): Function(getSignature()) { }

ast::ExpressionValue Strcmp::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t addr1 = params[0].get<uint32_t>();
    const uint32_t addr2 = params[1].get<uint32_t>();

    vm::MemoryView view1 = context->getMemory()->getView(addr1);
    vm::MemoryView view2 = context->getMemory()->getView(addr2);

    uint32_t index = 0;
    while (true) {
        const uint8_t b1 = view1.read<uint8_t>(index);
        const uint8_t b2 = view2.read<uint8_t>(index);

        if (b1 != b2)
            return ExpressionValue(TypeDecl::INT, (b1 < b2 ? -1 : 1));
        if (b1 == 0) 
            return ExpressionValue(TypeDecl::INT, 0);

        index++;
    }
}



}

}
}
}
