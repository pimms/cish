#include "DeclarationContext.h"


namespace cish
{
namespace ast
{


DeclarationContext::DeclarationContext():
    _insideFunction(false)
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
    if (!_insideFunction) {
        Throw(InvalidDeclarationScope, "Cannot push a variable scope outside a function");
    }

    _varScope.push_back(VariableScope());

}

void DeclarationContext::popVariableScope()
{
    const int minScopes = 1 + (_insideFunction ? 1 : 0);
    if (_varScope.size() <= minScopes) {
        Throw(InvalidDeclarationScope, "Cannot pop past root scope of context");
    }

    _varScope.pop_back();
}

void DeclarationContext::enterFunction()
{
    if (_insideFunction) {
        Throw(InvalidDeclarationScope, "Cannot enter a function while inside a function");
    }

    _insideFunction = true;
    _varScope.push_back(VariableScope());
}

void DeclarationContext::exitFunction()
{
    if (!_insideFunction) {
        Throw(InvalidDeclarationScope, "Cannot exit function when not inside a function");
    }

    if (_varScope.size() != 2) {
        Throw(InvalidDeclarationScope, "Cannot exit function - there are stacked scopes");
    }

    _varScope.pop_back();
    _insideFunction = false;
}

void DeclarationContext::declareFunction(FuncDeclaration func)
{
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

void DeclarationContext::pushSuperStatement(SuperStatement *super)
{
    _superStack.push_back(super);
}

void DeclarationContext::popSuperStatement()
{
    if (_superStack.empty()) {
        Throw(Exception, "SuperStatement underflow");
    }

    _superStack.pop_back();
}

SuperStatement* DeclarationContext::getCurrentSuper() const
{
    if (_superStack.empty())
        return nullptr;
    return _superStack.back();
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


}
}
