#include "AddrofExpression.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


AddrofExpression::AddrofExpression(Lvalue::Ptr lvalue):
    _lvalue(lvalue),
    _type(TypeDecl::getPointer(_lvalue->getType()))
{
}

ExpressionValue AddrofExpression::evaluate(vm::ExecutionContext *context) const
{
    return ExpressionValue(_type, _lvalue->getMemoryView(context).getAddress());
}

TypeDecl AddrofExpression::getType() const
{
    return _type;
}


}
}
