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
    typedef std::shared_ptr<ModuleContext> Ptr;
    static Ptr create();

    void addModule(Module::Ptr module);
    const Module::Ptr getModule(const std::string &name) const;

private:
    std::map<std::string, Module::Ptr> _modules;
};


}
}
