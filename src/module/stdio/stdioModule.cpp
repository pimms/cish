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
    Module::Ptr module = Module::Ptr(new Module("stdio.h"));
    module->addFunction(Function::Ptr(new impl::Puts()));
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
    return FuncDeclaration {
        TypeDecl::INT,
        "puts", {
            VarDeclaration {
                TypeDecl::getPointer(TypeDecl::getConst(TypeDecl::CHAR)),
                "ptr"
            }
        }
    };
}

Puts::Puts(): Function(getSignature()) {}

ast::ExpressionValue Puts::execute(vm::ExecutionContext *context,
                                   FuncParams params) const
{
    const uint32_t addr = params[0].get<uint32_t>();
    MemoryView view = context->getMemory()->getView(addr);

    std::vector<char> str;
    utils::readString(view, str);

    (*context->getStdout()) << str.data();

    return ExpressionValue(TypeDecl::INT, str.size());
}


}
}
}
}
