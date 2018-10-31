#include "Type.h"
#include "../Exception.h"

namespace cish
{
namespace ast
{

TypeDecl::TypeDecl(): _type(VOID) {}

TypeDecl::TypeDecl(const TypeDecl &o): _type(o._type) {}

TypeDecl::TypeDecl(Type t): _type(t) {}

TypeDecl::Type TypeDecl::getType() const
{
    return _type;
}

uint32_t TypeDecl::getSize() const
{
    switch (_type) {
        case VOID:
            return 0;
        case BOOL:
            return 1;
        case CHAR:
            return 1;
        case SHORT:
            return 2;
        case INT:
            return 4;
        case LONG:
            return 8;
        case FLOAT:
            return 4;
        case DOUBLE:
            return 8;
    }

    Throw(Exception, "Type '%d' has undefined size", (int)_type);
}

const char* TypeDecl::getName() const
{
    switch (_type) {
        case VOID:
            return "void";
        case BOOL:
            return "bool";
        case CHAR:
            return "char";
        case SHORT:
            return "short";
        case INT:
            return "int";
        case LONG:
            return "long";
        case FLOAT:
            return "float";
        case DOUBLE:
            return "double";
    }

    return "<undef>";
}

bool TypeDecl::operator==(const TypeDecl &o) const
{
    return _type == o.getType();
}

bool TypeDecl::operator==(const TypeDecl::Type &o) const
{
    return _type == o;
}

bool TypeDecl::castableTo(const TypeDecl &o) const
{
    return castableTo(o.getType());
}

bool TypeDecl::castableTo(const TypeDecl::Type &t) const
{
    if ((_type == VOID) != (t == VOID))
        return false;

    // TOOD: This is (probably?) incorrect, fix at some point
    return true;
}

bool TypeDecl::isIntegral() const
{
    return  _type == BOOL ||
            _type == CHAR ||
            _type == SHORT ||
            _type == INT ||
            _type == LONG;
}

bool TypeDecl::isFloating() const
{
    return  _type == FLOAT ||
            _type == DOUBLE;
}


template<> TypeDecl TypeDecl::getFromNative<void>() { return TypeDecl(TypeDecl::VOID); }
template<> TypeDecl TypeDecl::getFromNative<bool>() { return TypeDecl(TypeDecl::BOOL); }
template<> TypeDecl TypeDecl::getFromNative<char>() { return TypeDecl(TypeDecl::CHAR); }
template<> TypeDecl TypeDecl::getFromNative<short>() { return TypeDecl(TypeDecl::SHORT); }
template<> TypeDecl TypeDecl::getFromNative<int>() { return TypeDecl(TypeDecl::INT); }
template<> TypeDecl TypeDecl::getFromNative<long>() { return TypeDecl(TypeDecl::LONG); }
template<> TypeDecl TypeDecl::getFromNative<float>() { return TypeDecl(TypeDecl::FLOAT); }
template<> TypeDecl TypeDecl::getFromNative<double>() { return TypeDecl(TypeDecl::DOUBLE); }





}
}
