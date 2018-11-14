#include "Type.h"
#include "../Exception.h"

#include "AstNodes.h"   // InvalidTypeException

#include <map>

namespace cish
{
namespace ast
{

TypeDecl TypeDecl::getFromString(const std::string &str)
{
    if (str.back() == '*') {
        TypeDecl refType = getFromString(str.substr(0, str.length() - 1));
        return getPointer(refType);
    }

    static const std::map<std::string,Type> map = {
        {"void", VOID},
        {"bool", BOOL},
        {"char", CHAR},
        {"short", SHORT},
        {"int", INT},
        {"long", LONG},
        {"float", FLOAT},
        {"double", DOUBLE},
    };

    if (!map.count(str)) {
        Throw(InvalidTypeException, "Cannot resolve type '%s'", str.c_str());
    }

    return TypeDecl(map.at(str));
}

TypeDecl TypeDecl::getPointer(Type referencedType)
{
    TypeDecl type;
    type._type = Type::POINTER;
    type._referencedType = new TypeDecl(referencedType);
    return type;
}

TypeDecl TypeDecl::getPointer(const TypeDecl &referencedType)
{
    TypeDecl type;
    type._type = Type::POINTER;
    type._referencedType = new TypeDecl(referencedType);
    return type;
}


TypeDecl::TypeDecl():
    _type(VOID),
    _referencedType(nullptr),
    _const(false)
 {}

TypeDecl::TypeDecl(const TypeDecl &o)
{
    *this = o;
}

TypeDecl::TypeDecl(Type t):
    _type(t),
    _referencedType(nullptr),
    _const(false)
{
    if (_type == POINTER) {
        Throw(Exception, "Pointer types cannot be constructed through TypeDecl::TypeDecl(Type)");
    }
}

TypeDecl::~TypeDecl()
{
    if (_referencedType)
        delete _referencedType;
}

TypeDecl& TypeDecl::operator=(const TypeDecl &o)
{
    _type = o._type;
    _const = o._const;
    if (o._referencedType != nullptr) {
        // NB! potentially recursive initialization
        assert(_type == POINTER);
        _referencedType = new TypeDecl(*o._referencedType);
    } else {
        _referencedType = nullptr;
    }
    return *this;
}

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
        case POINTER:
            return 4;
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
        case POINTER:
            return "pointer (todo)";
    }

    return "<undef>";
}

const TypeDecl* TypeDecl::getReferencedType() const
{
    if (_type != Type::POINTER) {
        Throw(InvalidTypeException, "Cannot get referenced type of non-pointer TypeDecl");
    }
    assert(_referencedType != nullptr);
    return _referencedType;
}

bool TypeDecl::operator==(const TypeDecl &o) const
{
    if (_type != Type::POINTER) {
        return _type == o._type;
    }

    if (o.getType() != Type::POINTER) {
        return false;
    }

    return (*_referencedType == *o._referencedType);
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

    if (_type == POINTER || t == POINTER) {
        Type other;
        if (_type == POINTER) {
            other = t;
        } else {
            other = _type;
        }

        switch (other) {
            case FLOAT:
            case DOUBLE:
                return false;
            default:
                return true;
        }
    }

    return true;
}

bool TypeDecl::isIntegral() const
{
    return  _type == BOOL ||
            _type == CHAR ||
            _type == SHORT ||
            _type == INT ||
            _type == LONG ||
            _type == POINTER;
}

bool TypeDecl::isFloating() const
{
    return  _type == FLOAT ||
            _type == DOUBLE;
}

bool TypeDecl::isConst() const
{
    return _const;
}

void TypeDecl::setConst(bool isConst)
{
    _const = isConst;
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
