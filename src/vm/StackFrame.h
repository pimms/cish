#pragma once

#include <map>
#include <string>

#include "Variable.h"


namespace cish
{
namespace vm
{

class StackFrame
{
public:
    StackFrame();
    StackFrame(const StackFrame *parent);
    ~StackFrame();

    void addVariable(const std::string &name, Variable *var);

    Variable* getVariable(const std::string &name) const;

private:
    const StackFrame *_parent;
    std::map<std::string, Variable*> _vars;
};

}
}
