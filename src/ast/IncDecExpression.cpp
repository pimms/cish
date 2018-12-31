#include "IncDecExpression.h"
#include "DeclarationContext.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Variable.h"


namespace cish
{
namespace ast
{
namespace internal
{

template<typename T>
static ExpressionValue evaluate(IncDecExpression::Operation op,
                                TypeDecl returnType,
                                vm::MemoryView *memoryView,
                                int delta)
{
    const T preValue = memoryView->read<T>();
    memoryView->write<T>(preValue + delta);
    const T postValue = memoryView->read<T>();

    if (op == IncDecExpression::POSTFIX_INCREMENT ||
        op == IncDecExpression::POSTFIX_DECREMENT) {
        return ExpressionValue(returnType, preValue);
    } else {
        return ExpressionValue(returnType, postValue);
    }
}

}


IncDecExpression::IncDecExpression(IncDecExpression::Operation operation,
                                   Lvalue::Ptr lvalue):
    _operation(operation),
    _lvalue(lvalue)
{
    _type = _lvalue->getType();

    if (!_type.isIntegral()) {
        Throw(InvalidTypeException,
            "Increment/decrement operator only allowed on integral types");
    }

    if (_type.isConst()) {
        Throw(InvalidOperationException, "Cannot alter a const lvalue");
    }
}

TypeDecl IncDecExpression::getType() const
{
    return _type;
}

ExpressionValue IncDecExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::MemoryView view = _lvalue->getMemoryView(context);
    const int delta = getMutationValue();

    switch (_type.getType()) {
        case TypeDecl::BOOL:
        case TypeDecl::CHAR:
            return internal::evaluate<uint8_t>(_operation, _type, &view, delta);
        case TypeDecl::SHORT:
            return internal::evaluate<uint16_t>(_operation, _type, &view, delta);
        case TypeDecl::INT:
        case TypeDecl::POINTER:
            return internal::evaluate<uint32_t>(_operation, _type, &view, delta);
        default:
            Throw(InvalidTypeException, "Type '%s not handled'", _type.getName());
    }
}

int IncDecExpression::getMutationValue() const
{
    int delta = 0;
    if (_operation == PREFIX_INCREMENT || _operation == POSTFIX_INCREMENT) {
        delta = 1;
    } else {
        delta = -1;
    }

    if (_type == TypeDecl::POINTER) {
        if (_type.getReferencedType()->getType() != TypeDecl::VOID) {
            delta *= _type.getReferencedType()->getSize();
        }
    }

    return delta;
}


}
}
