#include "DeclarationContext.h"

#include <set>


namespace cish
{
namespace ast
{


DeclarationContext::DeclarationContext():
    _currentFunction(nullptr)
{
    _varScope.push_back(VariableScope());
}

void DeclarationContext::declareVariable(TypeDecl type, const std::string &name)
{
    checkForReservedKeyword(name);

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
    if (!_currentFunction) {
        Throw(InvalidDeclarationScope, "Cannot push a variable scope outside a function");
    }

    _varScope.push_back(VariableScope());

}

void DeclarationContext::popVariableScope()
{
    const int minScopes = 1 + (_currentFunction ? 1 : 0);
    if (_varScope.size() <= minScopes) {
        Throw(InvalidDeclarationScope, "Cannot pop past root scope of context");
    }

    _varScope.pop_back();
}

void DeclarationContext::enterFunction(FunctionDefinition::Ptr funcDef)
{
    if (_currentFunction) {
        Throw(InvalidDeclarationScope, "Cannot enter a function while inside a function");
    }

    _currentFunction = funcDef;
    _varScope.push_back(VariableScope());
}

void DeclarationContext::exitFunction()
{
    if (!_currentFunction) {
        Throw(InvalidDeclarationScope, "Cannot exit function when not inside a function");
    }

    if (_varScope.size() != 2) {
        Throw(InvalidDeclarationScope, "Cannot exit function - there are stacked scopes");
    }

    _varScope.pop_back();
    _currentFunction = nullptr;
}

FunctionDefinition::Ptr DeclarationContext::getCurrentFunction() const
{
    return _currentFunction;
}

void DeclarationContext::declareFunction(FuncDeclaration func)
{
    checkForReservedKeyword(func.name);

    const FuncDeclaration *existing = getFunctionDeclaration(func.name);
    if (existing != nullptr) {
        verifyIdenticalDeclarations(existing, &func);
    }

    _funcs[func.name] = func;
}

const FuncDeclaration* DeclarationContext::getFunctionDeclaration(const std::string &name) const
{
    if (_funcs.count(name) == 0)
        return nullptr;
    return &_funcs.at(name);
}

void DeclarationContext::verifyIdenticalDeclarations(const FuncDeclaration *existing, const FuncDeclaration *redecl)
{
    if (existing->returnType != redecl->returnType) {
        Throw(FunctionAlreadyDeclaredException,
              "Redeclaration of return-type of function '%s'. "
              "Already declared type '%s', then redeclared with return type '%s'",
              redecl->name.c_str(),
              existing->returnType.getName(),
              redecl->returnType.getName());
    }

    if (existing->params.size() != redecl->params.size()) {
        Throw(FunctionAlreadyDeclaredException,
              "Function '%s' redeclared with different number of parameters",
              redecl->name.c_str());
    }

    for (int i=0; i<(int)redecl->params.size(); i++) {
        if (redecl->params[i].type != existing->params[i].type) {
            Throw(FunctionAlreadyDeclaredException,
                    "Parameter %d ('%s') to function '%s' redeclared as type '%s'",
                    i+1,
                    redecl->params[i].type.getName(),
                    redecl->name.c_str(),
                    existing->params[i].type.getName());
        }
    }

}

void DeclarationContext::checkForReservedKeyword(const std::string &identifier) const
{
    static const std::set<std::string> reservedKeywords = {
        "sizeof", "for", "do", "while", "switch", "case", "goto",
        "bool", "char", "short", "int", "long", "float", 
        "double", "void", "unsigned"
    };

    if (reservedKeywords.count(identifier) != 0) {
        Throw(InvalidIdentifierException, "Identifier '%s' is reserved", identifier.c_str());
    }
}


}
}
