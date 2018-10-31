#include "VariableReferenceExpression.h"

#include "DeclarationContext.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


VariableReferenceExpression::VariableReferenceExpression(DeclarationContext *context, const std::string &varName)
{
    auto varDecl = context->getVariableDeclaration(varName);
    if (varDecl == nullptr) {
        Throw(VariableNotDeclaredException, "Variable '%s' not declared in current scope", varName.c_str());
    }

    _name = varName;
    _type = varDecl->type;
}

TypeDecl VariableReferenceExpression::getType() const
{
    return _type;
}

ExpressionValue VariableReferenceExpression::evaluate(vm::ExecutionContext *ctx)
{
    vm::Variable *var = ctx->getStackFrame()->getVariable(_name);
    if (!var) {
        Throw(VariableNotDefinedException, "Variable '%s' undefined", _name.c_str());
    }

    if (var->getType() != _type) {
        Throw(InvalidTypeException,
              "Expected variable with type '%s', found '%s'",
              _type.getName(), var->getType().getName());
    }

    const vm::Allocation *alloc = var->getAllocation();

    switch (_type.getType()) {
        case TypeDecl::BOOL:
            return ExpressionValue(_type, alloc->read<bool>());
        case TypeDecl::CHAR:
            return ExpressionValue(_type, alloc->read<char>());
        case TypeDecl::SHORT:
            return ExpressionValue(_type, alloc->read<short>());
        case TypeDecl::INT:
            return ExpressionValue(_type, alloc->read<int>());
        case TypeDecl::LONG:
            return ExpressionValue(_type, alloc->read<long>());
        case TypeDecl::FLOAT:
            return ExpressionValue(_type, alloc->read<float>());
        case TypeDecl::DOUBLE:
            return ExpressionValue(_type, alloc->read<double>());

        default:
            Throw(InvalidTypeException, "Unable to evaluate variable with type '%s'", _type.getName());
    }
}


}
}

