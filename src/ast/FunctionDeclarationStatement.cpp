#include "FunctionDeclarationStatement.h"

namespace cish
{
namespace ast
{


FunctionDeclarationStatement::FunctionDeclarationStatement(DeclarationContext *context,
                                                           FuncDeclaration decl):
    Statement(context->getCurrentSuper()),
    _decl(decl)
{
    context->declareFunction(_decl);
}

void FunctionDeclarationStatement::execute(vm::ExecutionContext*) const
{
    // Nothing to do, lol
}


}
}
