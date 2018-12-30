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
DECLARE_EXCEPTION(IncompleteTypeException);

DECLARE_EXCEPTION(StructFinalizedException);


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

/**
 * StructLayouts must be built incrementally in order to handle recursive
 * dynamic structures of the form:
 *      struct type_t { int n; struct type_t *next; };
 *
 * The general use case around this is that the StructLayout is passed to
 * the DeclarationContext before any fields have been added, so that fields
 * referring to the struct are valid.
 *
 * This does however NOT permit recursive static structures of the form:
 *      struct type_t { int n; struct type_t next; };
 */
class StructLayout
{
public:
    StructLayout(const std::string &name);

    void addField(TypeDecl type, const std::string &name);
    void finalize();

    bool isFinalized() const;
    const std::string& getName() const;
    const StructField* getField(const std::string &name) const;
    uint32_t getSize() const;

private:
    std::string _name;
    std::vector<StructField> _fields;
    std::map<const std::string, uint32_t> _fieldLookup;
    uint32_t _size;
    bool _finalized;
};


}
}
