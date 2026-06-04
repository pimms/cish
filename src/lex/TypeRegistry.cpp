#include "TypeRegistry.h"

namespace cish::lex
{

TypeRegistry::TypeRegistry()
{
    _primitives = {
        { "char" },
        { "unsigned char" },
        { "short" },
        { "unsigned short" },
        { "int" },
        { "unsigned int" },
        { "long" },
        { "unsigned long" },
        { "long long" },
        { "unsigned long long" },
        { "float" },
        { "double" },
        { "long double" },
        { "bool" },
        { "void" },

    };
}
void TypeRegistry::registerStruct(const std::string &structName)
{
    _structs.push_back(structName);
}

} // namespace cish::lex
