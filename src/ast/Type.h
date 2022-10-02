#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "StructLayout.h"


namespace cish::ast
{

class DeclarationContext;

class TypeDecl
{
public:
    enum Type
    {
        // Note: the order here is vital, as expressions of different types
        // must be cast to the type of the highest Type-value when binary
        // operations/operands are applied.
        //
        // A comparison between a bool and a float requires that the
        // boolean expression is cast to float, for instance.
        VOID,
        BOOL,
        CHAR,
        SHORT,
        INT,
        LONG,

        FLOAT,
        DOUBLE,

        POINTER,

        // Note: STRUCT-typed ExpressionValues always contains the memory address
        // of the structure object. We never pass the struct-buffer itself.
        STRUCT,
    };

    template<typename T>
    static TypeDecl getFromNative();
    static TypeDecl getFromString(const std::string &str);
    static TypeDecl getFromTokens(const DeclarationContext *declContext, const std::vector<std::string>& tokens);
    static TypeDecl getPointer(Type referencedType);
    static TypeDecl getPointer(const TypeDecl &referencedType);
    static TypeDecl getConst(const TypeDecl &type);
    static TypeDecl getStruct(const StructLayout* structLayout);


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

    // This method is ONLY valid if this type is a STRUCT
    const StructLayout* getStructLayout() const;

    bool operator==(const TypeDecl &o) const;
    bool operator==(const TypeDecl::Type &o) const;

    bool operator!=(const TypeDecl &o) const { return !(*this == o); }
    bool operator!=(const TypeDecl::Type &o) const { return !(*this == o); }

    bool castableTo(const TypeDecl &o) const;

    bool isIntegral() const;
    bool isFloating() const;

    bool isConst() const;
    void setConst(bool isConst);

private:
    Type _type;
    mutable std::shared_ptr<TypeDecl> _referencedType;
    bool _const;
    const StructLayout* _structLayout;
};


template<> TypeDecl TypeDecl::getFromNative<void>();
template<> TypeDecl TypeDecl::getFromNative<bool>();
template<> TypeDecl TypeDecl::getFromNative<char>();
template<> TypeDecl TypeDecl::getFromNative<short>();
template<> TypeDecl TypeDecl::getFromNative<int>();
template<> TypeDecl TypeDecl::getFromNative<float>();
template<> TypeDecl TypeDecl::getFromNative<double>();

}
