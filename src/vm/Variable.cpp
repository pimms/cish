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
    return _allocation->getAddress();
}


}
}
