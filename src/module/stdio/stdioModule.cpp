#include "stdioModule.h"

#include "../Utils.h"
#include "../../ast/Type.h"
#include "../../vm/Allocation.h"
#include "../../vm/ExecutionContext.h"


namespace cish
{
namespace module
{
namespace stdio
{



Module::Ptr buildModule()
{
    Module::Ptr module = Module::create("stdio.h");
    module->addFunction(Function::Ptr(new impl::Puts()));
    module->addFunction(Function::Ptr(new impl::Printf()));
    return module;
}


namespace impl
{


using ast::FuncDeclaration;
using ast::TypeDecl;
using ast::VarDeclaration;
using ast::ExpressionValue;
using vm::MemoryView;


/*
==================
int puts(const char *s)
==================
*/
ast::FuncDeclaration Puts::getSignature()
{
    return FuncDeclaration(
        TypeDecl::INT,
        "puts",
        {
            VarDeclaration {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "ptr"
            }
        }
    );
}

Puts::Puts(): Function(getSignature()) {}

ast::ExpressionValue Puts::execute(vm::ExecutionContext *context,
                                   FuncParams params,
                                   vm::Variable*) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    MemoryView view = context->getMemory()->getView(addr);

    std::vector<char> str;
    utils::readString(view, str);

    context->getStdout()->write(str.data());
    context->getStdout()->write("\n");

    return ExpressionValue(TypeDecl::INT, str.size());
}


/*
==================
int printf(const char *s, ...)
==================
*/
ast::FuncDeclaration Printf::getSignature()
{
    return FuncDeclaration(
        TypeDecl::INT,
        "printf",
        {
            VarDeclaration {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "s"
            }
        },
        true
    );
}

Printf::Printf(): Function(getSignature()) {}

ast::ExpressionValue Printf::execute(vm::ExecutionContext *context,
                                     FuncParams params,
                                     vm::Variable*) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    vm::Memory *memory = context->getMemory();
    MemoryView view = memory->getView(addr);

    std::vector<char> str;
    utils::readString(view, str);

    std::stringstream ss;

    int i = 0;
    int paramIndex = 1;
    while (i < str.size()) {
        char &ch = str[i++];
        if (ch != '%') {
            ss << ch;
        } else {
            if (i == str.size() - 1) {
                Throw(StdioException, "Bad format string: '%s'", str.data());
            }

            char &next = str[i++];
            if (next == '%') {
                ss << '%';
            } else {
                if (paramIndex >= params.size()) {
                    Throw(StdioException,
                        "Not enough parameters given for format-string '%s'",
                        str.data());
                }

                ExpressionValue value = params[paramIndex++];
                switch (next) {
                    case 'd':
                        ss << value.get<int>();
                        break;
                    case 'p':
                        ss << utils::hexstr(value.get<int>(), true);
                        break;
                    case 'x':
                        ss << utils::hexstr(value.get<int>(), false);
                        break;
                    case 'u':
                        ss << value.get<uint32_t>();
                        break;
                    case 's': {
                        std::vector<char> substr;
                        const uint32_t substrAddr = value.get<uint32_t>();
                        MemoryView subView = memory->getView(substrAddr);
                        utils::readString(subView, substr);
                        ss << substr.data();
                        break;
                    }
                    case 'g': // FALLTHROUGH
                    case 'f': {
                        if (value.getIntrinsicType() == TypeDecl::DOUBLE) {
                            ss << value.get<double>();
                        } else {
                            ss << value.get<float>();
                        }
                        break;
                    }
                    case 'c':
                        ss << value.get<char>();
                        break;
                    default:
                        Throw(StdioException,
                              "Bad format char '%c' in format string  '%s'",
                              next, str.data());
                }
            }
        }
    }

    std::string result = ss.str();
    context->getStdout()->write(result);
    return ExpressionValue(TypeDecl::INT, result.length());
}


}
}
}
}
