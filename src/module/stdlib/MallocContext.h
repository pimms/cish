#pragma once

#include "../../vm/Allocation.h"

#include <map>
#include <memory>


namespace cish
{
namespace module
{
namespace stdlib
{


class MallocContext
{
public:
    typedef std::shared_ptr<MallocContext> Ptr;

    void onAllocation(vm::Allocation::Ptr);
    bool attemptDeallocation(uint32_t addr);

private:
    std::map<uint32_t, vm::Allocation::Ptr> _allocations;
};


}
}
}
