#pragma once

#include <vector>
#include <map>

#include "Type.h"
#include "../Exception.h"
#include "SuperStatement.h"
#include "VarDeclaration.h"
#include "FuncDeclaration.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(VariableAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDefinedException);

DECLARE_EXCEPTION(InvalidDeclarationScope);


class FunctionDefinition;

class DeclarationContext
{
public:

    DeclarationContext();

    void declareVariable(TypeDecl type, const std::string &name);

    // The returned VarDeclaration - if non-null, may be deallocated
    // after the next call to 'popVariableScope', so take a copy if
    // you want to keep any of the values.
    const VarDeclaration* getVariableDeclaration(const std::string &name) const;

    void pushVariableScope();
    void popVariableScope();

    void enterFunction(FunctionDefinition *funcDef);
    void exitFunction();
    FunctionDefinition* getCurrentFunction() const;

    void declareFunction(FuncDeclaration decl);
    const FuncDeclaration* getFunctionDeclaration(const std::string &name) const;

private:
    typedef std::vector<VarDeclaration> VariableScope;
    std::vector<VariableScope> _varScope;
    FunctionDefinition *_currentFunction;
    std::map<std::string, FuncDeclaration> _funcs;
    std::vector<SuperStatement*> _superStack;

    VarDeclaration* findInScope(const std::string &name, VariableScope *scope);
    void verifyIdenticalDeclarations(const FuncDeclaration *existing, const FuncDeclaration *redecl);
};


}
}
