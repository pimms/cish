#include "DerefExpression.h"

#include "IncDecExpression.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Memory.h"
#include "../vm/Variable.h"



namespace cish
{
namespace ast
{


DerefExpression::DerefExpression(DeclarationContext *context, Expression::Ptr expression):
    _expression(expression)
{
    const bool isPointer = _expression->getType() == TypeDecl::POINTER;

    if (!isPointer) {
        Throw(InvalidOperationException, "Can only dereference pointers");
    }
}

ExpressionValue DerefExpression::evaluate(vm::ExecutionContext *context) const
{
    TypeDecl type = getType();
    ExpressionValue value = _expression->evaluate(context);

    uint32_t address = 0;
    address = value.get<uint32_t>();

    vm::Memory *memory = context->getMemory();
    vm::MemoryView view = memory->getView(address);
    switch (type.getType()) {
        case TypeDecl::BOOL:
            return ExpressionValue(type, view.read<bool>());
        case TypeDecl::CHAR:
            return ExpressionValue(type, view.read<char>());
        case TypeDecl::SHORT:
            return ExpressionValue(type, view.read<short>());
        case TypeDecl::POINTER:
            return ExpressionValue(type, view.read<uint32_t>());
        case TypeDecl::INT:
            return ExpressionValue(type, view.read<int32_t>());
        case TypeDecl::FLOAT:
            return ExpressionValue(type, view.read<float>());
        case TypeDecl::DOUBLE:
            return ExpressionValue(type, view.read<double>());

        default:
            Throw(InvalidTypeException, "Unable to dereference type '%s'", type.getName());
    }
}

TypeDecl DerefExpression::getType() const
{
    return *_expression->getType().getReferencedType();
}



}
}

