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
    const FunctionDefinition* getFunctionDefinition(const std::string &funcName);
    std::vector<const FunctionDefinition*> getFunctionDefinitions() const;

    void addRootStatement(Statement *statement);
    std::vector<const Statement*> getRootStatements() const;

private:
    std::vector<Statement*> _rootStatements;
    std::map<std::string,FunctionDefinition*> _funcDefs;
};


}
}
