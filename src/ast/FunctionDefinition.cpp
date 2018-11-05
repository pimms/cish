#include "FunctionDefinition.h"


namespace cish
{
namespace ast
{


FunctionDefinition::FunctionDefinition(DeclarationContext *context,
                                       FuncDeclaration decl,
                                       std::vector<Statement*> statements):
    _decl(decl),
    _statements(statements)
{
    context->declareFunction(decl);

    // TODO: Consider verifying that we actually return someting (if the returntype is
    // non-void), but I don't believe it's in any of the C-standards, so maybe just don't.
}

FunctionDefinition::~FunctionDefinition()
{
    for (Statement *s: _statements) {
        delete s;
    }
}

const FuncDeclaration* FunctionDefinition::getDeclaration() const
{
    return &_decl;
}



}
}
