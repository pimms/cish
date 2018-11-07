#pragma once

#include <map>
#include <string>

#include "Variable.h"


namespace cish
{
namespace vm
{

class Scope
{
public:
    Scope();
    Scope(const Scope *parent);
    ~Scope();

    void addVariable(const std::string &name, Variable *var);
    Variable* getVariable(const std::string &name) const;

private:
    const Scope *_parent;
    std::map<std::string, Variable*> _vars;
};

}
}
