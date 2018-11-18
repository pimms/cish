#pragma once

#include "Type.h"
#include <string>

namespace cish
{
namespace ast
{

struct VarDeclaration
{
    TypeDecl type;
    std::string name;
};


}
}