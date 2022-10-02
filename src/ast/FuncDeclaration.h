#pragma once

#include "Type.h"
#include "VarDeclaration.h"

#include <string>
#include <vector>

namespace cish::ast
{

struct FuncDeclaration
{
    FuncDeclaration()
    {
        // backwards compatible default-value
        varargs = false;
    }

    FuncDeclaration(TypeDecl type,
                    std::string name,
                    std::vector<VarDeclaration> params = {},
                    bool varargs = false)
    {
        this->returnType = type;
        this->name = name;
        this->params = params;
        this->varargs = varargs;
    }

    TypeDecl returnType;
    std::string name;
    std::vector<VarDeclaration> params;


    // When true, the function allows any number of parameters of any
    // type in addition to the explicitly defined ones. The explicit
    // parameters must however still be provided.
    //
    // E.g., a call to the function 'printf(const char *, ...)' cannot
    // be called with zero parameters, but it can be called with more
    // than one.
    bool varargs;
};

}
