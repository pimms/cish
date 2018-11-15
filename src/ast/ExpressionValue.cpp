#include "ExpressionValue.h"

#include <regex>


namespace cish
{
namespace ast
{



ExpressionValue::ExpressionValue(const std::string& rawValue)
{
    const std::regex exprInt("[0-9]+");
    const std::regex exprFloat("(\\.[0-9]+|[0-9]+(\\.[0-9]+)?)[fF]?");

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
    } else if (std::regex_match(rawValue, exprFloat)) {
        _type = TypeDecl(TypeDecl::FLOAT);
        _value.fval = std::atof(rawValue.c_str());
    } else {
        Throw(ExpressionTypeException, "Unable to determine intrinsic type of expression '%s'", rawValue.c_str());
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
}
