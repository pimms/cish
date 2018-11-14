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
                                vm::Variable *var,
                                int delta)
{
    const T preValue = var->getAllocation()->read<T>();
    var->getAllocation()->write<T>(preValue + delta);
    const T postValue = var->getAllocation()->read<T>();

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
    _varDecl(*context->getVariableDeclaration(varName))
{
    if (!_varDecl.type.isIntegral()) {
        Throw(InvalidTypeException,
            "Increment/decrement operator only allowed on integral types");
    }
}

IncDecExpression::~IncDecExpression()
{
}

TypeDecl IncDecExpression::getType() const
{
    return _varDecl.type;
}

ExpressionValue IncDecExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::Variable *var = context->getScope()->getVariable(_varDecl.name);
    if (var->getType() != _varDecl.type) {
        Throw(InvalidTypeException,
            "Expected type '%s', got '%s'",
            _varDecl.type.getName(), var->getType().getName());
    }

    const int delta = getMutationValue();

    switch (_varDecl.type.getType()) {
        case TypeDecl::BOOL:
        case TypeDecl::CHAR:
            return internal::evaluate<uint8_t>(_operation, _varDecl.type, var, delta);
        case TypeDecl::SHORT:
            return internal::evaluate<uint16_t>(_operation, _varDecl.type, var, delta);
        case TypeDecl::INT:
        case TypeDecl::POINTER:
            return internal::evaluate<uint32_t>(_operation, _varDecl.type, var, delta);
        case TypeDecl::LONG:
            return internal::evaluate<uint64_t>(_operation, _varDecl.type, var, delta);
        default:
            Throw(InvalidTypeException, "Type '%s not handled'", _varDecl.type.getName());
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

    if (_varDecl.type == TypeDecl::POINTER) {
        if (_varDecl.type.getReferencedType()->getType() != TypeDecl::VOID) {
            delta *= _varDecl.type.getReferencedType()->getSize();
        }
    }

    return delta;
}


}
}
