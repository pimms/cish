#pragma once

#include <vector>

#include "Type.h"
#include "../Exception.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(VariableAlreadyDeclaredException);

class DeclarationContext
{
public:
    struct VarDeclaration
    {
        const std::string name;
        TypeDecl type;
    };

    DeclarationContext();

    void declareVariable(const std::string &name, TypeDecl type);

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
