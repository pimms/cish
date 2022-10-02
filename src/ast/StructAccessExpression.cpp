#include "StructAccessExpression.h"
#include "StructField.h"
#include "../vm/ExecutionContext.h"

namespace cish::ast
{

StructAccessExpression::StructAccessExpression(Expression::Ptr structExpr,
                                               const std::string &memberName,
                                               AccessType accessType):
    _expression(structExpr)
{
    TypeDecl type = structExpr->getType();

    if (type == TypeDecl::STRUCT) {
        _struct = type.getStructLayout();
        if (accessType != OBJECT) {
            Throw(InvalidAccessException, "Struct must be accessed with the '.' operator");
        }
    } else if (type == TypeDecl::POINTER && type.getReferencedType()->getType() == TypeDecl::STRUCT) {
        _struct = type.getReferencedType()->getStructLayout();
        if (accessType != POINTER) {
            Throw(InvalidAccessException, "Pointer to struct must be accessed with the '->' operator");
        }
    } else {
        Throw(InvalidTypeException, "Struct access can only be performed on struct or pointer-to-struct expressions");
    }

    _field = _struct->getField(memberName);
}

TypeDecl StructAccessExpression::getType() const
{
    return _field->getType();
}

vm::MemoryView StructAccessExpression::getMemoryView(vm::ExecutionContext *context) const
{
    ExpressionValue value = _expression->evaluate(context);
    const uint32_t base = value.get<uint32_t>();
    const uint32_t offset = _struct->getField(_field->getName())->getOffset();
    const uint32_t addr = (base + offset);

    return context->getMemory()->getView(addr);
}

}
