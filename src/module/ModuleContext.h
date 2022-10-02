#pragma once

#include "Module.h"

#include <map>
#include <string>


namespace cish::module
{

class ModuleContext
{
public:
    typedef std::unique_ptr<ModuleContext> Ptr;
    static Ptr create();

    void addModule(Module::Ptr module);
    const Module::Ptr getModule(const std::string &name) const;
    const std::vector<Module::Ptr> getModules() const;

private:
    std::map<std::string, Module::Ptr> _modules;
};

}
