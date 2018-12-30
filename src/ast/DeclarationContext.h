#pragma once

#include <vector>
#include <map>

#include "Type.h"
#include "../Exception.h"
#include "SuperStatement.h"
#include "VarDeclaration.h"
#include "FuncDeclaration.h"
#include "FunctionDefinition.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(VariableAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDeclaredException);
DECLARE_EXCEPTION(StructAlreadyDeclaredException);
DECLARE_EXCEPTION(FunctionAlreadyDefinedException);
DECLARE_EXCEPTION(InvalidIdentifierException);

DECLARE_EXCEPTION(InvalidDeclarationScope);


class FunctionDefinition;
class StructLayout;

class DeclarationContext
{
public:

    DeclarationContext();
    ~DeclarationContext();

    void declareVariable(TypeDecl type, const std::string &name);

    // The returned VarDeclaration - if non-null, may be deallocated
    // after the next call to 'popVariableScope', so take a copy if
    // you want to keep any of the values.
    const VarDeclaration* getVariableDeclaration(const std::string &name) const;

    // The DeclarationContext takes ownership of the StructLayout.
    void declareStruct(StructLayout *structLayout);
    const StructLayout* getStruct(const std::string &name) const;

    void pushVariableScope();
    void popVariableScope();

    void enterFunction(FunctionDefinition::Ptr funcDef);
    void exitFunction();
    FunctionDefinition::Ptr getCurrentFunction() const;

    void declareFunction(FuncDeclaration decl);
    const FuncDeclaration* getFunctionDeclaration(const std::string &name) const;

private:
    typedef std::vector<VarDeclaration> VariableScope;
    std::vector<VariableScope> _varScope;
    FunctionDefinition::Ptr _currentFunction;
    std::map<std::string, FuncDeclaration> _funcs;
    std::map<std::string, StructLayout*> _structs;

    VarDeclaration* findInScope(const std::string &name, VariableScope *scope);
    void verifyIdenticalDeclarations(const FuncDeclaration *existing, const FuncDeclaration *redecl);
    void checkForReservedKeyword(const std::string &identifier) const;
    void checkIdentifierLength(const std::string &identifier) const;
};


}
}
