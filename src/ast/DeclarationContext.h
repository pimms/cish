#pragma once

#include <vector>

#include "Type.h"
#include "../Exception.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(VariableAlreadyDeclaredException);


struct VarDeclaration
{
    std::string name;
    TypeDecl type;
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

private:
    typedef std::vector<VarDeclaration> VariableScope;
    std::vector<VariableScope> _varScope;
};


}
}
