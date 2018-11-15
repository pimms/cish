#include "DerefExpression.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Memory.h"
#include "../vm/Variable.h"



namespace cish
{
namespace ast
{


DerefExpression::DerefExpression(DeclarationContext *context, const std::string &varName)
{
    const VarDeclaration *decl = context->getVariableDeclaration(varName);
    if (decl == nullptr) {
        Throw(VariableNotDeclaredException, "Variable '%s' not declared in current scope", varName.c_str());
    }

    if (decl->type != TypeDecl::POINTER) {
        Throw(InvalidOperationException, "Can only dereference pointers");
    }

    _varDecl = *decl;
}

ExpressionValue DerefExpression::evaluate(vm::ExecutionContext *context) const
{
    TypeDecl type = getType();

    vm::Variable *var = context->getScope()->getVariable(_varDecl.name);
    const uint32_t ptrAddr = var->getAllocation()->read<uint32_t>();

    vm::Memory *memory = context->getMemory();
    vm::MemoryView view = memory->getView(ptrAddr);
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
    return *_varDecl.type.getReferencedType();
}



}
}

