#include "ParseTree.h"
#include "../Visitor.h"

#include <variant>
#include <cassert>

namespace cish::parse
{

static std::string baseTypeToString(const BaseType& t)
{
    return std::visit(Visitor {
        [](const std::string& s) -> std::string {
            return s;
        },
        [](const BuiltInType& bt) -> std::string {
            switch (bt) {
               case BuiltInType::BOOL: return "BOOL";
               case BuiltInType::VOID: return "VOID";
               case BuiltInType::CHAR: return "CHAR";
               case BuiltInType::UCHAR: return "UCHAR";
               case BuiltInType::SHORT: return "SHORT";
               case BuiltInType::USHORT: return "USHORT";
               case BuiltInType::INT: return "INT";
               case BuiltInType::UINT: return "UINT";
               case BuiltInType::LINT: return "LINT";
               case BuiltInType::ULINT: return "ULINT";
               case BuiltInType::LLINT: return "LLINT";
               case BuiltInType::ULLINT: return "DOUBLE";
               case BuiltInType::FLOAT: return "FLOAT";
               case BuiltInType::DOUBLE: return "DOUBLE";
               case BuiltInType::LDOUBLE: return "LDOUBLE";
            }
            assert(0);
            return "UNHANDLED";
       }
    }, t);
}

std::string TypeIdentifier::toString() const
{
    return std::format("{}{}{}{}",
        isConst  ? "const " : "",
        isStruct ? "struct " : "",
        baseTypeToString(baseType),
        std::string(static_cast<size_t>(pointerLevel), '*')
    );
}
}
