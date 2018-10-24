#include "StackFrame.h"


namespace cish
{
namespace vm
{

StackFrame::StackFrame(): StackFrame(nullptr) { }

StackFrame::StackFrame(const StackFrame *parent): _parent(parent) { }

StackFrame::~StackFrame()
{
    for (const std::pair<std::string, Variable*> &pair: _vars) {
        delete pair.second;
    }
}


void StackFrame::addVariable(const std::string &name, Variable *var)
{
    // Delete existing variable if one exists
    if (_vars.count(name) != 0) {
        delete _vars[name];
    }

    _vars[name] = var;
}

Variable* StackFrame::getVariable(const std::string &name) const
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
}
