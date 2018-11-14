#include "AddrofExpression.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


AddrofExpression::AddrofExpression(DeclarationContext *context, const std::string &varName)
{
    const VarDeclaration *decl = context->getVariableDeclaration(varName);
    if (decl == nullptr) {
        Throw(VariableNotDeclaredException, "Variable '%s' not declared in current scope", varName.c_str());
    }

    _varDecl = *decl;
}

ExpressionValue AddrofExpression::evaluate(vm::ExecutionContext *context) const
{
    vm::Variable *var = context->getScope()->getVariable(_varDecl.name);
    TypeDecl type = getType();
    return ExpressionValue(type, var->getHeapAddress());
}

TypeDecl AddrofExpression::getType() const
{
    return TypeDecl::getPointer(_varDecl.type);
}


}
}
