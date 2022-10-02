#include "StringEscape.h"

#include <map>
#include <sstream>

namespace cish::ast::string
{

static const ::std::map<char,char> g_charEscapeMap = {
    {'\\', '\\'},
    {'n', '\n'},
    {'0', '\0'},
    {'t', '\t'},
    {'"', '"'},
    {'\'', '\''},
};

std::string unescapeString(const std::string &string)
{
    std::stringstream ss;

    int i=0;
    while (i < string.length()) {
        const char ch = string[i];
        if (ch == '\\') {
            if (i == string.length() - 1) {
                Throw(StringEscapeException,
                    "Out of bounds excaping in string literal '%s'",
                    string.c_str());
            }

            const char next = string[i + 1];

            if (g_charEscapeMap.count(next)) {
                ss << g_charEscapeMap.at(next);
            } else {
                Throw(StringEscapeException,
                    "Unable to escape character '%c' in string '%s'",
                    next, string.c_str());
            }
            i += 2;
        } else {
            ss << ch;
            i++;
        }
    }

    std::string result = ss.str();
    return result;
}

int8_t unescapeChar(const std::string &rawCharLiteral)
{
    const std::string content = rawCharLiteral.substr(1, rawCharLiteral.length() - 2);

    if (content.length() == 1)
        return content[0];

    if (content[0] != '\\' || content.length() != 2)
        Throw(StringEscapeException, "Invalid character literal '%s'", content.c_str());

    const char ch = content[1];
    if (!g_charEscapeMap.count(ch))
        Throw(StringEscapeException, "Cannot escape character '%c'", ch);

    return g_charEscapeMap.at(ch);
}

}