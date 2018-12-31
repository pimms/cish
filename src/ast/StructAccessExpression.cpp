#include "StructAccessExpression.h"


namespace cish
{
namespace ast
{


StructAccessExpression::StructAccessExpression(Expression::Ptr structExpr,
                                               const std::string &memberName,
                                               AccessType accessType)
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
    Throw(Exception, "TODO");
}


}
}
