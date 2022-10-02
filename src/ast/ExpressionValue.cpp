#include "ExpressionValue.h"
#include "StringEscape.h"

#include <regex>
#include <map>


namespace cish::ast
{

ExpressionValue::ExpressionValue(const std::string& rawValue)
{
    const std::regex exprInt("[-]?[0-9]+");
    const std::regex exprHex("0[xX][0-9a-fA-F]+");
    const std::regex exprFloat("[-]?(\\.[0-9]+|[0-9]+(\\.[0-9]*)?)[fF]?");
    const std::regex exprChar("'(\\\\.|[^'\\\\])'");

    memset(&_value, 0, sizeof(_value));

    if (rawValue == "true") {
        _type = TypeDecl::BOOL;
        _value.bval = true;
    } else if (rawValue == "false") {
        _type = TypeDecl::BOOL;
        _value.bval = false;
    } else if (rawValue == "NULL") {
        _type = TypeDecl::INT;
        _value.ival = 0;
    } else if (std::regex_match(rawValue, exprInt)) {
        _type = TypeDecl(TypeDecl::INT);
        _value.ival = std::atoi(rawValue.c_str());
    } else if (std::regex_match(rawValue, exprHex)) {
        _type = TypeDecl(TypeDecl::INT);
        _value.ival = std::strtol(rawValue.c_str()+2, nullptr, 16);
    } else if (std::regex_match(rawValue, exprFloat)) {
        _type = TypeDecl(TypeDecl::FLOAT);
        _value.fval = std::atof(rawValue.c_str());
    } else if (std::regex_match(rawValue, exprChar)) {
        _type = TypeDecl(TypeDecl::CHAR);
        _value.chval = string::unescapeChar(rawValue);
    } else {
        Throw(ExpressionTypeException, "Unable to determine intrinsic type of expression: %s", rawValue.c_str());
    }
}

ExpressionValue::ExpressionValue(int intValue)
{
    _type = TypeDecl(TypeDecl::INT);
    _value.ival = intValue;
}

ExpressionValue::ExpressionValue(float floatValue)
{
    _type = TypeDecl(TypeDecl::FLOAT);
    _value.fval = floatValue;
}

ExpressionValue::ExpressionValue()
{
    memset(&_value, 0, sizeof(_value));
}

const TypeDecl& ExpressionValue::getIntrinsicType() const
{
    return _type;
}

}
