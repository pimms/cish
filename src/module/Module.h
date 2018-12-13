#pragma once

#include "Function.h"

#include <string>
#include <memory>


namespace cish
{
namespace module
{


class Module
{
public:
    typedef std::shared_ptr<Module> Ptr;

    static Ptr create(const std::string &name);

    Module(const std::string &name);
    ~Module();

    void addFunction(Function::Ptr function);
    void addDependency(const std::string &dep);

    std::string getName() const;
    std::vector<Function::Ptr> getFunctions() const;
    std::vector<std::string> getDependencies() const;

private:
    std::string _name;
    std::vector<Function::Ptr> _functions;
    std::vector<std::string> _dependencies;
};


}
}
