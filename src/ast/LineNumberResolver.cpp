#include "LineNumberResolver.h"

namespace cish
{
namespace ast
{


LineNumberResolver::LineNumberResolver(const std::string &source)
{
    uint32_t charIndex = 0;
    _firstCharMap.push_back(0);

    for (int i=0; i<source.length(); i++) {
        if (source[i] == '\n') {
            _firstCharMap.push_back(i+1);
        }
    }

}

uint32_t LineNumberResolver::resolveCharIndex(uint32_t charIndex) const
{
    auto it = _firstCharMap.begin() + 1;
    const auto end = _firstCharMap.end();

    int line = 1;
    while (it != end && charIndex >= (*it)) {
        line++;
        it++;
    }

    return line;
}


}
}
