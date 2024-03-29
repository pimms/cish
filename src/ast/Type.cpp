#include "Type.h"
#include "../Exception.h"

#include "AstNodes.h"   // InvalidTypeException
#include "DeclarationContext.h"

#include <stack>
#include <map>


namespace cish::ast
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
        {"long", LONG},
        {"float", FLOAT},
        {"double", DOUBLE},
    };

    if (!map.count(str)) {
        Throw(InvalidTypeException, "Cannot resolve type '%s'", str.c_str());
    }

    return TypeDecl(map.at(str));
}

TypeDecl TypeDecl::getFromTokens(const DeclarationContext *declContext, const std::vector<std::string> &tokens)
{
    auto it = tokens.begin();
    bool constant = false;
    if (*it == "const") {
        constant = true;
        it++;
    }

    if (it == tokens.end())
        Throw(Exception, "too few tokens to work with");

    TypeDecl type;

    if (*it == "struct") {
        it++;
        type = TypeDecl::getStruct(declContext->getStruct(*it));
    } else {
        type = getFromString(*it);
    }

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
    type._referencedType = std::make_shared<TypeDecl>(referencedType);
    return type;
}

TypeDecl TypeDecl::getPointer(const TypeDecl &referencedType)
{
    TypeDecl type;
    type._type = Type::POINTER;
    type._referencedType = std::make_shared<TypeDecl>(referencedType);
    return type;
}

TypeDecl TypeDecl::getConst(const TypeDecl &type)
{
    TypeDecl copy = type;
    copy.setConst(true);
    return copy;
}

TypeDecl TypeDecl::getStruct(const StructLayout *structLayout)
{
    TypeDecl type;
    type._structLayout = structLayout;
    type._type = TypeDecl::STRUCT;
    return type;
}


TypeDecl::TypeDecl():
    _type(VOID),
    _referencedType(nullptr),
    _const(false),
    _structLayout(nullptr)
 {}

TypeDecl::TypeDecl(const TypeDecl &o):
    _referencedType(nullptr)
{
    *this = o;
}

TypeDecl::TypeDecl(Type t):
    _type(t),
    _referencedType(nullptr),
    _const(false),
    _structLayout(nullptr)
{
    if (_type == POINTER) {
        Throw(Exception, "Pointer types cannot be constructed through TypeDecl::TypeDecl(Type)");
    } else if (_type == STRUCT) {
        Throw(Exception, "Struct types cannot be constructed through TypeDecl::TypeDecl(Type)");
    }
}

TypeDecl::~TypeDecl()
{
}

TypeDecl& TypeDecl::operator=(const TypeDecl &o)
{
    _type = o._type;
    _const = o._const;
    _structLayout = o._structLayout;
    if (o._referencedType != nullptr) {
        // NB! potentially recursive initialization
        assert(_type == POINTER);
        _referencedType = o._referencedType;
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
        case STRUCT:
            return _structLayout->getSize();
    }

    Throw(Exception, "Type '%d' has undefined size", (int)_type);
}

const char* TypeDecl::getName() const
{
    if (_type == POINTER || _type == STRUCT) {
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
            case LONG:
                return "long";
            case FLOAT:
                return "float";
            case DOUBLE:
                return "double";
            default:
                return "<error>";
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
            case LONG:
                return "const long";
            case FLOAT:
                return "const float";
            case DOUBLE:
                return "const double";
            default:
                return "<error>";
        }
    }

    return "<undef>";
}

const TypeDecl* TypeDecl::getReferencedType() const
{
    if (_type != POINTER) {
        Throw(InvalidTypeException, "Cannot get referenced type of non-pointer TypeDecl");
    }
    assert(_referencedType != nullptr);
    return _referencedType.get();
}

const StructLayout* TypeDecl::getStructLayout() const
{
    if (_type != STRUCT) {
        Throw(InvalidTypeException, "Cannot get struct layout of non-struct TypeDecl");
    }
    assert(_structLayout != nullptr);
    return _structLayout;
}

bool TypeDecl::operator==(const TypeDecl &o) const
{
    if (_type != Type::POINTER) {
        return _type == o._type;
    }

    if (o.getType() != Type::POINTER) {
        return false;
    }

    if (_type == Type::STRUCT) {
        return _type == o._type
            && _structLayout->getName() == o._structLayout->getName();
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
        /* Both of the types are pointers */

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
    } else if (_type == POINTER || t == POINTER) {
        /* One of the types is a pointer */
        Type nonPointer;
        if (_type == POINTER) {
            nonPointer = t;
        } else {
            nonPointer = _type;
        }

        switch (nonPointer) {
            case FLOAT:
            case DOUBLE:
            case VOID:
            case STRUCT:
                return false;
            default:
                return true;
        }
    } else if (_type == STRUCT) {
        return o._type == STRUCT
            && _structLayout->getName() == o._structLayout->getName();
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


static const char* getPointerName(const TypeDecl *type)
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

static const char* getStructName(const TypeDecl *type)
{
    static const int STRLEN = 100;
    static const int numNameBufs = 15;
    static char nameBufs[numNameBufs][STRLEN] = {{""}};
    static std::atomic_int nextIndex = 0;

    const uint32_t bufIdx = (nextIndex++) % numNameBufs;
    char *buf = nameBufs[bufIdx];

    buf = stpcpy(buf, "struct ");
    buf = stpcpy(buf, type->getStructLayout()->getName().c_str());
    return nameBufs[bufIdx];
}

static const char* getComplexName(const TypeDecl *type)
{
    if (type->getType() == TypeDecl::POINTER) {
        return getPointerName(type);
    } else if (type->getType() == TypeDecl::STRUCT) {
        return getStructName(type);
    } else {
        return type->getName();
    }
}

}
