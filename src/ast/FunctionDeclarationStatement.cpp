#include "FunctionDeclarationStatement.h"
#include "../vm/ExecutionContext.h"

namespace cish
{
namespace ast
{


FunctionDeclarationStatement::FunctionDeclarationStatement(DeclarationContext *context,
                                                           FuncDeclaration decl):
    _decl(decl)
{
    context->declareFunction(_decl);
}

void FunctionDeclarationStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;
}


}
}
