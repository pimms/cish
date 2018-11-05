#include "VariableAssignmentStatement.h"

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
        const std::string &varName,
        Expression *value):
    _varName(varName),
    _expression(value)
{
    auto var = context->getVariableDeclaration(_varName);
    if (var == nullptr) {
        Throw(VariableNotDeclaredException, "Variable '%s' not declared in current context", _varName.c_str());
    }

    if (!value->getType().castableTo(var->type)) {
        Throw(InvalidCastException, "Cannot cast value of '%s' into variable '%s' of type '%s'",
                value->getType().getName(), var->name.c_str(), var->type.getName());
    }
}

VariableAssignmentStatement::~VariableAssignmentStatement()
{
    delete _expression;
}

void VariableAssignmentStatement::execute(vm::ExecutionContext *context)
{
    vm::Variable *var = context->getStackFrame()->getVariable(_varName);
    if (var == nullptr) {
        Throw(VariableNotDefinedException, "Variable '%s' not defined", _varName.c_str());
    }

    // TODO: Ensure variable is not const

    ExpressionValue value = _expression->evaluate(context);

    switch (var->getType().getType()) {
        case TypeDecl::BOOL:
            var->getAllocation()->write(value.get<bool>());
            break;
        case TypeDecl::CHAR:
            var->getAllocation()->write(value.get<char>());
            break;
        case TypeDecl::SHORT:
            var->getAllocation()->write(value.get<short>());
            break;
        case TypeDecl::INT:
            var->getAllocation()->write(value.get<int>());
            break;
        case TypeDecl::LONG:
            var->getAllocation()->write(value.get<long>());
            break;
        case TypeDecl::FLOAT:
            var->getAllocation()->write(value.get<float>());
            break;
        case TypeDecl::DOUBLE:
            var->getAllocation()->write(value.get<double>());
            break;

        default:
            Throw(InvalidTypeException, "Cannot assign to variable of type '%s'", var->getType().getName());
    }
}


}
}