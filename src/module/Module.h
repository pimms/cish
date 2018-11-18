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

    Module(const std::string &name);
    ~Module();

    void addFunction(Function::Ptr function);

    std::string getName() const;
    std::vector<Function::Ptr> getFunctions() const;

private:
    std::string _name;
    std::vector<Function::Ptr> _functions;
};


}
}