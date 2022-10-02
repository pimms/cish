#include "StringLiteralExpression.h"
#include "../vm//ExecutionContext.h"

namespace cish::ast
{

StringLiteralExpression::StringLiteralExpression(StringId stringId):
    _stringId(stringId)
{
    TypeDecl constChar = TypeDecl::CHAR;
    constChar.setConst(true);
    _type = TypeDecl::getPointer(constChar);
}

ExpressionValue StringLiteralExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::MemoryView view = context->resolveString(_stringId);
    return ExpressionValue(ExpressionValue(_type, view.getAddress()));
}

TypeDecl StringLiteralExpression::getType() const
{
    return _type;
}

}