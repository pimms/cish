#include "Type.h"
#include "../Exception.h"

#include "AstNodes.h"   // InvalidTypeException

#include <stack>
#include <map>


namespace cish
{
namespace ast
{

static const char* getComplexName(const TypeDecl *td);



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
        {"float", FLOAT},
        {"double", DOUBLE},
    };

    if (!map.count(str)) {
        Throw(InvalidTypeException, "Cannot resolve type '%s'", str.c_str());
    }

    return TypeDecl(map.at(str));
}

TypeDecl TypeDecl::getFromTokens(const std::vector<std::string> &tokens)
{
    auto it = tokens.begin();
    bool constant = false;
    if (*it == "const") {
        constant = true;
        it++;
    }

    if (it == tokens.end())
        Throw(Exception, "too few tokens to work with");

    TypeDecl type = getFromString(*it);
    type.setConst(constant);
    it++;

    while (it != tokens.end()) {
        if (*it != "*")
            Throw(Exception, "Unexpected token: %s", it->c_str());
        type = getPointer(type);
        it++;
    }

    return type;
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

TypeDecl TypeDecl::getConst(const TypeDecl &type)
{
    TypeDecl copy = type;
    copy.setConst(true);
    return copy;
}


TypeDecl::TypeDecl():
    _type(VOID),
    _referencedType(nullptr),
    _const(false)
 {}

TypeDecl::TypeDecl(const TypeDecl &o):
    _referencedType(nullptr)
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
        if (_referencedType)
            delete _referencedType;
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
    if (_type == POINTER) {
        return getComplexName(this);
    }

    if (isConst() == false) {
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
            case FLOAT:
                return "float";
            case DOUBLE:
                return "double";
            case POINTER:
                return "<pointer>";
        }
    } else {
        switch (_type) {
            case VOID:
                return "const void";
            case BOOL:
                return "const bool";
            case CHAR:
                return "const char";
            case SHORT:
                return "const short";
            case INT:
                return "const int";
            case FLOAT:
                return "const float";
            case DOUBLE:
                return "const double";
            case POINTER:
                return "<const pointer>";
        }
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
    Type t = o.getType();

    if ((_type == VOID) != (t == VOID))
        return false;

    if (_type == POINTER && t == POINTER) {
        // Only check for const-compatibility, we don't
        // care about the actual types at this point.
        const TypeDecl *myInner = getReferencedType();
        const TypeDecl *theirInner = o.getReferencedType();

        while (myInner->getType() == Type::POINTER && myInner->getReferencedType())
            myInner = myInner->getReferencedType();
        while (theirInner->getType() == Type::POINTER && theirInner->getReferencedType())
            theirInner = theirInner->getReferencedType();

        if (myInner->isConst())
            return theirInner->isConst();
        return true;
    } if (_type == POINTER || t == POINTER) {
        Type other;
        if (_type == POINTER) {
            other = t;
        } else {
            other = _type;
        }

        switch (other) {
            case FLOAT:
            case DOUBLE:
            case VOID:
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
template<> TypeDecl TypeDecl::getFromNative<float>() { return TypeDecl(TypeDecl::FLOAT); }
template<> TypeDecl TypeDecl::getFromNative<double>() { return TypeDecl(TypeDecl::DOUBLE); }




static const char* getComplexName(const TypeDecl *type)
{
    static const int STRLEN = 100;
    static const int numNameBufs = 15;
    static char nameBufs[numNameBufs][STRLEN] = {{""}};
    static std::atomic_int nextIndex = 0;

    const uint32_t bufIdx = (nextIndex++) % numNameBufs;
    char *buf = nameBufs[bufIdx];

    std::stack<const TypeDecl*> stack;
    stack.push(type);

    while (stack.top()->getType() == TypeDecl::POINTER) {
        stack.push(stack.top()->getReferencedType());
    }

    while (stack.size()) {
        const TypeDecl *td = stack.top();
        stack.pop();

        if (td->getType() == TypeDecl::POINTER) {
            buf = stpcpy(buf, "*");
        } else {
            buf = stpcpy(buf, td->getName());
        }

        if (buf - nameBufs[bufIdx] >= STRLEN-5) {
            buf = stpcpy(nameBufs[bufIdx] + (STRLEN-5), "...");
            break;
        }
    }

    return nameBufs[bufIdx];
}


}
}
