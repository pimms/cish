#include "ParseTree.h"

#include <format>

namespace cish::parse
{

std::string TypeIdentifier::toString() const
{
    return std::format("{}{}{}{}",
        isConst  ? "const " : "",
        isStruct ? "struct " : "",
        type,
        std::string(static_cast<size_t>(pointerLevel), '*')
    );
}
}
