#pragma once

#include <stdint.h>
#include <vector>
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

        FLOAT,
        DOUBLE,

        POINTER,
    };

    template<typename T>
    static TypeDecl getFromNative();
    static TypeDecl getFromString(const std::string &str);
    static TypeDecl getFromTokens(const std::vector<std::string>& tokens);
    static TypeDecl getPointer(Type referencedType);
    static TypeDecl getPointer(const TypeDecl &referencedType);


    TypeDecl();
    TypeDecl(const TypeDecl &o);
    TypeDecl(Type t);
    ~TypeDecl();

    TypeDecl& operator=(const TypeDecl &o);

    Type getType() const;
    uint32_t getSize() const;
    const char* getName() const;

    // This method is ONLY valid if this type is a POINTER.
    const TypeDecl* getReferencedType() const;

    bool operator==(const TypeDecl &o) const;
    bool operator==(const TypeDecl::Type &o) const;

    bool operator!=(const TypeDecl &o) const { return !(*this == o); }
    bool operator!=(const TypeDecl::Type &o) const { return !(*this == o); }

    bool castableTo(const TypeDecl &o) const;
    bool castableTo(const TypeDecl::Type &o) const;

    bool isIntegral() const;
    bool isFloating() const;

    bool isConst() const;
    void setConst(bool isConst);

private:
    Type _type;
    TypeDecl *_referencedType;
    bool _const;
};


template<> TypeDecl TypeDecl::getFromNative<void>();
template<> TypeDecl TypeDecl::getFromNative<bool>();
template<> TypeDecl TypeDecl::getFromNative<char>();
template<> TypeDecl TypeDecl::getFromNative<short>();
template<> TypeDecl TypeDecl::getFromNative<int>();
template<> TypeDecl TypeDecl::getFromNative<float>();
template<> TypeDecl TypeDecl::getFromNative<double>();


}
}
