#include "StringTable.h"
#include "MurmurHash2.h"
#include "../Exception.h"

namespace cish::ast
{

static const uint32_t g_hseed = 0x8BADF00D;

StringTable::Ptr StringTable::create()
{
    return StringTable::Ptr(new StringTable());
}

StringTable::StringTable():
    _nextStringId(1)
{

}

StringId StringTable::insert(const std::string &str)
{
    const uint32_t hash = MurmurHash2(str, g_hseed);
    if (_definedStrings.find(hash) == _definedStrings.end()) {
        const StringId id = _nextStringId++;
        _definedStrings.insert(hash);
        _map[id] = str;
        return id;
    }

    for (const auto& pair: _map) {
        if (pair.second == str) {
            return pair.first;
        }
    }

    Throw(Exception,
        "Internal inconsistency - failed to find string with H=%d (%s) in lookup table",
        hash, str.c_str());
}

std::string StringTable::resolve(StringId id) const
{
    if (!_map.count(id)) {
        Throw(Exception, "String reference not found: %d", id);
    }

    return _map.at(id);
}

const std::map<StringId,std::string>& StringTable::getMap() const
{
    return _map;
}

}