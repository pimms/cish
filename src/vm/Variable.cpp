#include "Variable.h"


namespace cish
{
namespace vm
{

Variable::Variable(Allocation::Ptr allocation):
    _allocation(std::move(allocation))
{
}

uint32_t Variable::getSize() const
{
    return _allocation->getSize();
}

uint32_t Variable::getHeapAddress() const
{
    return _allocation->getOffset();
}


}
}
