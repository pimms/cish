#pragma once

#include <stdint.h>
#include <vector>
#include <string>

namespace cish
{
namespace ast
{


class LineNumberResolver
{
public:
    LineNumberResolver(const std::string &source);

    uint32_t resolveCharIndex(uint32_t charIndex) const;

private:
    std::vector<uint32_t> _firstCharMap;
};


}
}
