#include "Module.h"


namespace cish::module
{

Module::Ptr Module::create(const std::string &name)
{
    return std::make_shared<Module>(name);
}

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

void Module::addDependency(const std::string &dep)
{
    _dependencies.push_back(dep);
}

void Module::addStruct(ast::StructLayout::Ptr structLayout)
{
    _structs.push_back(structLayout);
}

std::string Module::getName() const
{
    return _name;
}

std::vector<Function::Ptr> Module::getFunctions() const
{
    return _functions;
}

std::vector<std::string> Module::getDependencies() const
{
    return _dependencies;
}

std::vector<ast::StructLayout::Ptr> Module::getStructs() const
{
    return _structs;
}

}
