#pragma once

#include "Module.h"

#include <map>
#include <string>


namespace cish
{
namespace module
{


class ModuleContext
{
public:
    void addModule(Module::Ptr module);
    const Module* getModule(const std::string &name) const;

private:
    std::map<std::string, Module::Ptr> _modules;
};


}
}
