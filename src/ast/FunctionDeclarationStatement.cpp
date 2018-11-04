#include "FunctionDeclarationStatement.h"

namespace cish
{
namespace ast
{


FunctionDeclarationStatement::FunctionDeclarationStatement(DeclarationContext *context, FuncDeclaration decl):
    _decl(decl)
{
    context->declareFunction(_decl);
}
    
void FunctionDeclarationStatement::execute(vm::ExecutionContext*)
{
    // Nothing to do, lol
}


}
}
