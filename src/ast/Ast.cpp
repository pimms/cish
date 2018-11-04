#include "Ast.h"

#include "Ast.h"


namespace cish
{
namespace ast
{

Ast::Ast()
{

}
    
Ast::~Ast()
{
    for (Statement *s: _rootStatements) {
        delete s;
    }
}
    
void Ast::addRootStatement(Statement *statement)
{
    assert(statement != nullptr);
    _rootStatements.push_back(statement);
}
    
const std::vector<Statement*>& Ast::getRootStatements() const
{
    return _rootStatements;
}


}
}

