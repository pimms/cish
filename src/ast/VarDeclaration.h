#pragma once

#include "Type.h"
#include <string>

namespace cish::ast
{

struct VarDeclaration
{
    TypeDecl type;
    std::string name;
};

}