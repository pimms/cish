#pragma once

#include <memory>

#include "Allocation.h"


namespace cish
{
namespace vm
{


class Variable
{
public:
    Variable(Allocation::Ptr allocation);

    uint32_t getSize() const;
    uint32_t getHeapAddress() const;

private:
    Allocation::Ptr _allocation;
};


}
}
