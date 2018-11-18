#pragma once

#include "AstNodes.h"
#include "FunctionDefinition.h"
#include "StringTable.h"
#include "../module/Module.h"
#include "../vm/Callable.h"

#include <memory>
#include <vector>
#include <map>


namespace cish
{
namespace ast
{


class Ast
{
public:
    typedef std::unique_ptr<Ast> Ptr;

    Ast();
    ~Ast();

    void addFunctionDefinition(vm::Callable::Ptr funcDef);
    void addModule(const module::Module::Ptr module);
    const vm::Callable::Ptr getFunctionDefinition(const std::string &funcName);
    std::vector<const vm::Callable::Ptr> getFunctionDefinitions() const;

    void addRootStatement(Statement *statement);
    std::vector<const Statement*> getRootStatements() const;

    void setStringTable(StringTable::Ptr stringTable);
    const StringTable* getStringTable() const;

private:
    std::vector<Statement*> _rootStatements;
    std::map<std::string,vm::Callable::Ptr> _funcDefs;
    StringTable::Ptr _stringTable;
};


}
}
