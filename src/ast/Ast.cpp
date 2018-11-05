#include "Ast.h"

#include "FunctionDefinition.h"
#include "FunctionDeclarationStatement.h"


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
    
    for (auto pair: _funcDefs) {
        delete pair.second;
    }
}
    
void Ast::addFunctionDefinition(FunctionDefinition *funcDef)
{
    const std::string funcName = funcDef->getDeclaration()->name;
    
    if (_funcDefs.count(funcName)) {
        Throw(FunctionAlreadyDefinedException,
              "Function '%s' has already been defined",
              funcName.c_str());
    }
    
    _funcDefs[funcName] = funcDef;
}

FunctionDefinition* Ast::getFunctionDefinition(const std::string &funcName)
{
    if (_funcDefs.count(funcName) == 0)
        return nullptr;
    return _funcDefs[funcName];
}

std::vector<FunctionDefinition*> Ast::getFunctionDefinitions() const
{
    std::vector<FunctionDefinition*> funcs;
    for (auto pair: _funcDefs) {
        funcs.push_back(pair.second);
    }
    
    return funcs;
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

