#pragma once

#include <string>
#include <vector>

namespace cish::lex {

class TypeRegistry
{
public:
    TypeRegistry();
    void registerStruct(const std::string& structName);

private:
    std::vector<std::vector<std::string>> _primitives;
    std::vector<std::string> _structs;
};

}