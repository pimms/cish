#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Type.h"
#include "VarDeclaration.h"

#include "../Exception.h"


namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(FieldAlreadyDeclaredException);
DECLARE_EXCEPTION(EmptyStructException);
DECLARE_EXCEPTION(NoSuchFieldException);

class StructField
{
public:
    StructField(const std::string &name, const TypeDecl &type, uint32_t offset);

    const std::string& getName() const;
    const TypeDecl& getType() const;
    const uint32_t getOffset() const;

private:
    std::string _name;
    TypeDecl _type;
    uint32_t _offset;
};

class StructLayout
{
public:
    StructLayout(const std::string &name, const std::vector<VarDeclaration> &fields);

    const std::string& getName() const;
    const StructField* getField(const std::string &name) const;
    uint32_t getSize() const;

private:
    std::string _name;
    std::vector<StructField> _fields;
    std::map<const std::string, uint32_t> _fieldLookup;
    uint32_t _size;
};


}
}
