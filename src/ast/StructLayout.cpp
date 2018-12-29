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
StructLayout::StructLayout(const std::string &name, const std::vector<VarDeclaration> &fields):
    _name(name),
    _size(0)
{
    uint32_t offset = 0;
    uint32_t index = 0;

    if (fields.size() == 0) {
        Throw(EmptyStructException, "Struct '%s' has no fields", name.c_str());
    }

    for (const VarDeclaration &decl: fields) {
        _fields.push_back(StructField(decl.name, decl.type, offset));
        offset += decl.type.getSize();
        _size += decl.type.getSize();

        if (_fieldLookup.count(decl.name) != 0) {
            Throw(FieldAlreadyDeclaredException, "Duplicate field with name '%s'", decl.name.c_str());
        }

        _fieldLookup[decl.name] = index++;
    }
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


}
}
