#pragma once

#include "../Exception.h"

#include <string>


namespace cish
{
namespace ast
{
namespace string
{


DECLARE_EXCEPTION(StringEscapeException);

/**
 * Converts a string on the format "\\nabc" to "\nabc"
 */
std::string unescapeString(const std::string &string);

/**
 * Converts a string on the format "'\\n'"" to the character
 * literal '\n'.
 */
int8_t unescapeChar(const std::string &rawCharLiteral);


}
}
}