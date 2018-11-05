#pragma once

#include <vector>
#include <map>

#include "Type.h"
#include "../Exception.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(VariableAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDefinedException);

DECLARE_EXCEPTION(InvalidDeclarationScope);


struct VarDeclaration
{
    TypeDecl type;
    std::string name;
};


struct FuncDeclaration
{
    TypeDecl returnType;
    std::string name;
    std::vector<VarDeclaration> params;
};


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

    void enterFunction();
    void exitFunction();

    void declareFunction(FuncDeclaration decl);
    const FuncDeclaration* getFunctionDeclaration(const std::string &name) const;

    SuperStatement *getCurrentSuper() const;

private:
    typedef std::vector<VarDeclaration> VariableScope;
    std::vector<VariableScope> _varScope;
    bool _insideFunction;

    std::map<std::string, FuncDeclaration> _funcs;

    VarDeclaration* findInScope(const std::string &name, VariableScope *scope);
    void verifyIdenticalDeclarations(const FuncDeclaration *existing, const FuncDeclaration *redecl);
};


}
}
