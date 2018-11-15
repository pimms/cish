#include "IncDecExpression.h"
#include "VariableReferenceExpression.h"
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


IncDecExpression::IncDecExpression(DeclarationContext *context,
                                   IncDecExpression::Operation operation,
                                   Lvalue *lvalue):
    _operation(operation),
    _lvalue(lvalue)
{
    if (!_lvalue->getType().isIntegral()) {
        Throw(InvalidTypeException,
            "Increment/decrement operator only allowed on integral types");
    }

    if (_lvalue->getType().isConst()) {
        Throw(InvalidOperationException, "Cannot alter a const variable");
    }
}

IncDecExpression::~IncDecExpression()
{
}

TypeDecl IncDecExpression::getType() const
{
    return _lvalue->getType();
}

ExpressionValue IncDecExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::MemoryView view = _lvalue->getMemoryView(context);
    const int delta = getMutationValue();

    const TypeDecl type = _lvalue->getType();
    switch (type.getType()) {
        case TypeDecl::BOOL:
        case TypeDecl::CHAR:
            return internal::evaluate<uint8_t>(_operation, type, &view, delta);
        case TypeDecl::SHORT:
            return internal::evaluate<uint16_t>(_operation, type, &view, delta);
        case TypeDecl::INT:
        case TypeDecl::POINTER:
            return internal::evaluate<uint32_t>(_operation, type, &view, delta);
        default:
            Throw(InvalidTypeException, "Type '%s not handled'", type.getName());
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

    const TypeDecl &type = _lvalue->getType();
    if (type == TypeDecl::POINTER) {
        if (type.getReferencedType()->getType() != TypeDecl::VOID) {
            delta *= type.getReferencedType()->getSize();
        }
    }

    return delta;
}


}
}
