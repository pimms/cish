#include "DeclarationContext.h"


namespace cish
{
namespace ast
{


DeclarationContext::DeclarationContext()
{
    _varScope.push_back(VariableScope());
}

void DeclarationContext::declareVariable(TypeDecl type, const std::string &name)
{
    for (const auto &var: _varScope.back()) {
        if (var.name == name) {
            Throw(VariableAlreadyDeclaredException,
                  "Variable '%s' is already declared in the current scope", name.c_str());
        }
    }

    _varScope.back().push_back(VarDeclaration { type, name });
}

const VarDeclaration* DeclarationContext::getVariableDeclaration(const std::string &name) const
{
    for (int i=_varScope.size() - 1; i >= 0; i--) {
        const VariableScope &scope = _varScope[i];
        for (int j=0; j<scope.size(); j++) {
            if (scope[j].name == name) {
                return &scope[j];
            }
        }
    }

    return nullptr;
}

void DeclarationContext::pushVariableScope()
{
    _varScope.push_back(VariableScope());
}

void DeclarationContext::popVariableScope()
{
    _varScope.pop_back();
}
    
void DeclarationContext::declareFunction(FuncDeclaration func)
{
    if (_funcs.count(func.name) != 0) {
        Throw(FunctionAlreadyDeclaredException, "Function '%s' has already been declared", func.name.c_str());
    }
    
    _funcs[func.name] = func;
}

const FuncDeclaration* DeclarationContext::getFunctionDeclaration(const std::string &name) const
{
    if (_funcs.count(name) == 0)
        return nullptr;
    return &_funcs.at(name);
}


}
}
