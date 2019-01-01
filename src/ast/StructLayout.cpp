#include "StructLayout.h"
#include "StructField.h"

namespace cish
{
namespace ast
{


StructLayout::StructLayout(const std::string &name):
    _name(name),
    _size(0),
    _finalized(false)
{
    if (_name == "") {
        Throw(Exception, "StructLayout must have a name");
    }
}

StructLayout::~StructLayout()
{
    for (StructField *field: _fields) {
        delete field;
    }
}

void StructLayout::addField(StructField *field)
{
    // Add the field into _fields ASAP to prevent leaks.
    // If something fails, we'll throw, and we want to ensure that the
    // field is actually deleted.
    const int index = _fields.size();
    _fields.push_back(field);

    field->setOffset(_size);
    _size += field->getType().getSize();

    if (_finalized) {
        Throw(StructFinalizedException,
              "Cannot add field '%s %s' to struct '%s' - already finalized",
              field->getType().getName(),
              field->getName().c_str(),
              _name.c_str());
    }

    if (field->getType().getType() == TypeDecl::STRUCT
        && !field->getType().getStructLayout()->isFinalized()) {
        Throw(IncompleteTypeException,
              "Cannot declare field of incomplete type '%s' in struct %s",
              field->getType().getName(),
              _name.c_str());
    }

    if (_fieldLookup.count(field->getName()) != 0) {
        Throw(FieldAlreadyDeclaredException, "Duplicate field with name '%s'",
                field->getName().c_str());
    }
    _fieldLookup[field->getName()] = index;
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
        return _fields.at(_fieldLookup.at(name));
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
