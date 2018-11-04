#pragma once

#include <stdint.h>
#include <string>


namespace cish
{
namespace ast
{

class TypeDecl
{
public:
    enum Type
    {
        // Note: the order here is vital, as expressions of different types
        // must be casted to the type of the highest Type-value.
        VOID,
        BOOL,
        CHAR,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
    };

    template<typename T>
    static TypeDecl getFromNative();
    static TypeDecl getFromString(const std::string &str);


    TypeDecl();
    TypeDecl(const TypeDecl &o);
    TypeDecl(Type t);

    Type getType() const;
    uint32_t getSize() const;
    const char* getName() const;

    bool operator==(const TypeDecl &o) const;
    bool operator==(const TypeDecl::Type &o) const;

    bool operator!=(const TypeDecl &o) const { return !(*this == o); }
    bool operator!=(const TypeDecl::Type &o) const { return !(*this == o); }

    bool castableTo(const TypeDecl &o) const;
    bool castableTo(const TypeDecl::Type &o) const;

    bool isIntegral() const;
    bool isFloating() const;

private:
    Type _type;
};


template<> TypeDecl TypeDecl::getFromNative<void>();
template<> TypeDecl TypeDecl::getFromNative<bool>();
template<> TypeDecl TypeDecl::getFromNative<char>();
template<> TypeDecl TypeDecl::getFromNative<short>();
template<> TypeDecl TypeDecl::getFromNative<int>();
template<> TypeDecl TypeDecl::getFromNative<long>();
template<> TypeDecl TypeDecl::getFromNative<float>();
template<> TypeDecl TypeDecl::getFromNative<double>();


}
}
