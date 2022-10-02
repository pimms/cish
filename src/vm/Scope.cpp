#include "Scope.h"


namespace cish::vm
{

Scope::Scope(): Scope(nullptr) { }

Scope::Scope(const Scope *parent): _parent(parent) { }

Scope::~Scope()
{
    for (const std::pair<std::string, Variable*> &pair: _vars) {
        delete pair.second;
    }
}


void Scope::addVariable(const std::string &name, Variable *var)
{
    // Delete existing variable if one exists
    if (_vars.count(name) != 0) {
        delete _vars[name];
    }

    _vars[name] = var;
}

Variable* Scope::getVariable(const std::string &name) const
{
    if (_vars.count(name) != 0) {
        return _vars.at(name);
    }

    if (_parent) {
        return _parent->getVariable(name);
    }

    return nullptr;
}

}
