#pragma once

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <map>

#include "Type.h"
#include "../Exception.h"

namespace cish::vm
{
class Variable;
}

namespace cish::ast
{

DECLARE_EXCEPTION(ExpressionTypeException);


class ExpressionValue
{
public:
    template<typename T>
    ExpressionValue(TypeDecl type, T value);

    ExpressionValue(const std::string& rawValue);
    ExpressionValue(int intValue);
    ExpressionValue(float floatValue);

    const TypeDecl& getIntrinsicType() const;

    template<typename T>
    T get() const;

    template<typename T>
    ExpressionValue cast() const;

private:
    /* used for late private init */
    ExpressionValue();

    TypeDecl _type;
    union {
        int64_t ival;
        int8_t chval;
        float fval;
        bool bval;
    } _value;
};


template<typename T>
ExpressionValue::ExpressionValue(TypeDecl type, T value)
{
    _type = type;
    memset(&_value, 0, sizeof(_value));

    if (std::is_same<bool,T>()) {
        _value.bval = value;
    } else if (type.isIntegral()) {
        _value.ival = value;
    } else if (type == TypeDecl::STRUCT) {
        _value.ival = value;
    } else if (type.isFloating()) {
        _value.fval = value;
    }
}

template<typename T>
T ExpressionValue::get() const
{
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
                  "T must be integral or floating");

    if constexpr (std::is_integral_v<T>) {
        if (_type.isFloating()) {
            return (T)_value.fval;
        }
        return (T)_value.ival;
    } else if (std::is_floating_point_v<T>) {
        if (_type.isFloating()) {
            return _value.fval;
        }
        return (T)_value.ival;
    }
}



template<typename T>
ExpressionValue ExpressionValue::cast() const
{
    ExpressionValue exprVal;

    T *tptr = (T*)&exprVal._value;
    *tptr = get<T>();

    exprVal._type = TypeDecl::getFromNative<T>();

    return exprVal;
}

}
