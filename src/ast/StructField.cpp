#include "StructField.h"
#include "StructLayout.h"

namespace cish
{
namespace ast
{


StructField::StructField(const TypeDecl &type, const std::string &name):
    _name(name),
    _type(type),
    _offset(0)
{
    if (_name == "") {
        Throw(Exception, "StructField must have a name");
    }
}

void StructField::setOffset(uint32_t offset)
{
    _offset = offset;
}

const std::string& StructField::getName() const
{
    return _name;
}

const TypeDecl& StructField::getType() const
{
    return _type;
}

const uint32_t StructField::getOffset() const
{
    return _offset;
}


}
}
