#include "Function.h"


namespace cish::module
{


Function::Function(ast::FuncDeclaration declaration):
    _declaration(declaration)
{

}

const ast::FuncDeclaration* Function::getDeclaration() const
{
    return &_declaration;
}

}
