#pragma once

#include "Type.h"
#include "VarDeclaration.h"

#include <string>
#include <vector>


namespace cish
{
namespace ast
{

struct FuncDeclaration
{
    TypeDecl returnType;
    std::string name;
    std::vector<VarDeclaration> params;
};


}
}
