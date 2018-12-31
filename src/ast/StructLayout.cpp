#include "StructLayout.h"

namespace cish
{
namespace ast
{


/*
===================
StructField
===================
*/
StructField::StructField(const std::string &name, const TypeDecl &type, uint32_t offset):
    _name(name),
    _type(type),
    _offset(offset)
{

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


/*
===================
StructLayout
===================
*/
StructLayout::StructLayout(const std::string &name):
    _name(name),
    _size(0),
    _finalized(false)
{
}

void StructLayout::addField(TypeDecl type, const std::string &name)
{
    if (_finalized) {
        Throw(StructFinalizedException,
              "Cannot add field '%s %s' to struct '%s' - already finalized",
              type.getName(), name.c_str(), _name.c_str());
    }

    if (type.getType() == TypeDecl::STRUCT && !type.getStructLayout()->isFinalized()) {
        Throw(IncompleteTypeException,
              "Cannot declare field of incomplete type '%s' in struct %s",
              type.getName(), _name.c_str());
    }

    const int index = _fields.size();

    _fields.push_back(StructField(name, type, _size));
    _size += type.getSize();

    if (_fieldLookup.count(name) != 0) {
        Throw(FieldAlreadyDeclaredException, "Duplicate field with name '%s'", name.c_str());
    }

    _fieldLookup[name] = index;
}

void StructLayout::finalize()
{
    if (_fields.empty()) {
        Throw(EmptyStructException, "Struct '%s' has no fields", _name.c_str());
    }

    _finalized = true;
}

bool StructLayout::isFinalized() const
{
    return _finalized;
}

const std::string& StructLayout::getName() const
{
    return _name;
}

const StructField* StructLayout::getField(const std::string &name) const
{
    if (_fieldLookup.count(name) != 0) {
        return &_fields.at(_fieldLookup.at(name));
    }

    Throw(NoSuchFieldException, "No field '%s' in struct %s", name.c_str(), _name.c_str());
    return nullptr;
}

uint32_t StructLayout::getSize() const
{
    return _size;
}


}
}
