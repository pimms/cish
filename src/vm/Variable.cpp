#include "Variable.h"


namespace cish
{
namespace vm
{

using cish::ast::TypeDecl;

Variable::Variable(TypeDecl type, Allocation::Ptr allocation):
    _type(type),
    _allocation(std::move(allocation))
{
    if (_type.getSize() > _allocation->getSize()) {
        Throw(InvalidAllocationException, 
              "Variable of type '%s' requires %d bytes, allocation only had %d",
              _type.getName(), _type.getSize(), _allocation->getSize());
    }
}

TypeDecl Variable::getType() const
{
    return _type;
}

Allocation* Variable::getAllocation() const
{
    return _allocation.get();
}

uint32_t Variable::getHeapAddress() const
{
    return _allocation->getOffset();
}


}
}
