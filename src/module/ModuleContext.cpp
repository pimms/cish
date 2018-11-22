#include "ModuleContext.h"


namespace cish
{
namespace module
{


ModuleContext::Ptr ModuleContext::create()
{
    return std::make_unique<ModuleContext>();
}

void ModuleContext::addModule(Module::Ptr module)
{
    _modules[module->getName()] = std::move(module);
}

const Module::Ptr ModuleContext::getModule(const std::string &name) const
{
    if (_modules.count(name) == 0)
        return nullptr;
    return _modules.at(name);
}


}
}