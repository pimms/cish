#include "VariableAssignmentStatement.h"
#include "DeclarationContext.h"

#include "../vm/Variable.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


/*
==============
VariableAssignmentStatement
==============
*/
VariableAssignmentStatement::VariableAssignmentStatement(
        DeclarationContext *context,
        Lvalue::Ptr lvalue,
        Expression::Ptr value,
        VariableAssignmentStatement::ConstAwareness constAwareness):
    _lvalue(lvalue),
    _expression(value)
{
    if (!value->getType().castableTo(_lvalue->getType())) {
        Throw(InvalidCastException, "Cannot cast value of '%s' to type '%s'",
                value->getType().getName(),
                _lvalue->getType().getName());
    }

    if (constAwareness == ConstAwareness::STRICT && _lvalue->getType().isConst()) {
        Throw(InvalidOperationException, "Cannot assign to a constant variable");
    }
}

void VariableAssignmentStatement::execute(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;

    Statement::execute(context);
    executeAssignment(context);
}


void VariableAssignmentStatement::executeAssignment(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;

    ExpressionValue value = _expression->evaluate(context);
    vm::MemoryView view = _lvalue->getMemoryView(context);

    switch (_lvalue->getType().getType()) {
        case TypeDecl::BOOL:
            view.write<bool>(value.get<bool>());
            break;
        case TypeDecl::CHAR:
            view.write<char>(value.get<char>());
            break;
        case TypeDecl::SHORT:
            view.write<short>(value.get<short>());
            break;
        case TypeDecl::INT:
            view.write<int32_t>(value.get<int32_t>());
            break;
        case TypeDecl::FLOAT:
            view.write<float>(value.get<float>());
            break;
        case TypeDecl::DOUBLE:
            view.write<double>(value.get<double>());
            break;
        case TypeDecl::POINTER:
            view.write<uint32_t>(value.get<uint32_t>());
            break;

        default:
            Throw(InvalidTypeException,
                "Cannot assign to variable of type '%s'",
                _lvalue->getType().getName());
    }
}


}
}
