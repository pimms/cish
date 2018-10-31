#pragma once

#include <memory>

#include "Allocation.h"
#include "../ast/Type.h"
#include "../Exception.h"


namespace cish
{

namespace vm
{

DECLARE_EXCEPTION(InvalidAllocationException);


class Variable
{
public:
    Variable(ast::TypeDecl type, Allocation::Ptr allocation);

    cish::ast::TypeDecl getType() const;
    Allocation* getAllocation() const;
    uint32_t getHeapAddress() const;

private:
    ast::TypeDecl _type;
    Allocation::Ptr _allocation;
};


}
}
