#include "Module.h"


namespace cish
{
namespace module
{


Module::Module(const std::string &name):
    _name(name)
{
}

Module::~Module()
{
}

void Module::addFunction(Function::Ptr function)
{
    _functions.push_back(function);
}

std::string Module::getName() const
{
    return _name;
}

std::vector<Function::Ptr> Module::getFunctions() const
{
    return _functions;
}


}
}