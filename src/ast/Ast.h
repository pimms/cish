#pragma once

#include "AstNodes.h"
#include "FunctionDefinition.h"
#include "StringTable.h"
#include "StructLayout.h"
#include "../module/Module.h"
#include "../vm/Callable.h"

#include <memory>
#include <vector>
#include <map>


namespace cish::ast
{

class Ast
{
public:
    typedef std::shared_ptr<Ast> Ptr;

    Ast();

    void addFunctionDefinition(vm::Callable::Ptr funcDef);
    void addModule(const module::Module::Ptr module);
    const vm::Callable::Ptr getFunctionDefinition(const std::string &funcName);
    std::vector<vm::Callable::Ptr> getFunctionDefinitions() const;

    void addRootStatement(Statement::Ptr statement);
    std::vector<Statement::Ptr> getRootStatements() const;

    void setStringTable(StringTable::Ptr stringTable);
    const StringTable* getStringTable() const;

    void addStructLayout(StructLayout::Ptr structLayout);

private:
    std::vector<Statement::Ptr> _rootStatements;
    std::map<std::string,vm::Callable::Ptr> _funcDefs;
    StringTable::Ptr _stringTable;

    // Structs are stored in the Ast purely for keeping the objects
    // alive throughout this program's lifecycle.
    std::vector<StructLayout::Ptr> _structLayouts;
};

}
