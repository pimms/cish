#pragma once

#include <string>
#include <stdint.h>

#include "Type.h"

namespace cish
{
namespace ast
{


class StructField
{
public:
    StructField(const TypeDecl &type, const std::string &name);

    void setOffset(uint32_t offset);

    const std::string& getName() const;
    const TypeDecl& getType() const;
    const uint32_t getOffset() const;

private:
    const std::string _name;
    const TypeDecl _type;
    uint32_t _offset;
};


}
}

