#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <set>
#include <memory>


namespace cish
{
namespace ast
{


typedef uint32_t StringId;

class StringTable
{
public:
    typedef std::unique_ptr<StringTable> Ptr;

    static Ptr create();
    StringTable();

    StringId insert(const std::string &str);
    std::string resolve(StringId id) const;

    const std::map<StringId,std::string>& getMap() const;

private:
    std::map<StringId,std::string> _map;
    std::set<uint32_t> _definedStrings;

    StringId _nextStringId;
};


}
}