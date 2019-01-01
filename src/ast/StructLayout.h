#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../Exception.h"


namespace cish
{
namespace ast
{

class StructField;

DECLARE_EXCEPTION(FieldAlreadyDeclaredException);
DECLARE_EXCEPTION(EmptyStructException);
DECLARE_EXCEPTION(NoSuchFieldException);
DECLARE_EXCEPTION(IncompleteTypeException);

DECLARE_EXCEPTION(StructFinalizedException);

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
    typedef std::shared_ptr<const StructLayout> Ptr;

    StructLayout(const std::string &name);
    ~StructLayout();

    void addField(StructField *field);
    void finalize();

    bool isFinalized() const;
    const std::string& getName() const;
    const StructField* getField(const std::string &name) const;
    uint32_t getSize() const;

private:
    const std::string _name;
    std::vector<StructField*> _fields;
    std::map<const std::string, uint32_t> _fieldLookup;
    uint32_t _size;
    bool _finalized;
};


}
}
