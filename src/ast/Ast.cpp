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

const FunctionDefinition* Ast::getFunctionDefinition(const std::string &funcName)
{
    if (_funcDefs.count(funcName) == 0)
        return nullptr;
    return _funcDefs[funcName];
}

std::vector<const FunctionDefinition*> Ast::getFunctionDefinitions() const
{
    std::vector<const FunctionDefinition*> funcs;
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

std::vector<const Statement*> Ast::getRootStatements() const
{
    std::vector<const Statement*> copy;
    for (Statement *s: _rootStatements)
        copy.push_back(s);
    return copy;
}

void Ast::setStringTable(StringTable::Ptr stringTable)
{
    _stringTable = std::move(stringTable);
}

const StringTable* Ast::getStringTable() const
{
    if (_stringTable == nullptr)
        return nullptr;
    return _stringTable.get();
}



}
}

