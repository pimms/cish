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
                                   const std::string &varName):
    _operation(operation),
    _varName(varName)
{
    const VarDeclaration *varDecl = context->getVariableDeclaration(_varName);
    if (varDecl == nullptr)
        Throw(VariableNotDeclaredException, "Variable '%s' not declared", _varName.c_str());

    _type = varDecl->type;

    if (!_type.isIntegral()) {
        Throw(InvalidTypeException,
            "Increment/decrement operator only allowed on integral types");
    }

    if (_type.isConst()) {
        Throw(InvalidOperationException, "Cannot alter a const variable");
    }
}

TypeDecl IncDecExpression::getType() const
{
    return _type;
}

ExpressionValue IncDecExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::Variable *var = context->getScope()->getVariable(_varName);
    if (var == nullptr || var->getType() != _type)
        Throw(Exception, "Unable to resolve var '%s' to expected type", _varName.c_str());

    vm::MemoryView view = *var->getAllocation();
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
