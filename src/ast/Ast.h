#pragma once

#include "AstNodes.h"
#include "FunctionDefinition.h"

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

    void addFunctionDefinition(FunctionDefinition *funcDef);
    FunctionDefinition* getFunctionDefinition(const std::string &funcName);
    std::vector<FunctionDefinition*> getFunctionDefinitions() const;

    void addRootStatement(Statement *statement);
    const std::vector<Statement*>& getRootStatements() const;

private:
    std::vector<Statement*> _rootStatements;
    std::map<std::string,FunctionDefinition*> _funcDefs;
};


}
}
