#include "DerefExpression.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Memory.h"
#include "../vm/Variable.h"



namespace cish
{
namespace ast
{


namespace internal
{

    template<typename T>
    ExpressionValue createExpressionValue(TypeDecl type, std::vector<uint8_t> &buffer)
    {
        if (buffer.size() != sizeof(T)) {
            Throw(InvalidCastException,
                  "Dereferenced type had unexpected size: %d (expected %d)",
                  sizeof(T), buffer.size());
        }

        const uint8_t *rawBuf = buffer.data();
        return ExpressionValue(type, *((T*)rawBuf));
    }

}



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
    std::vector<uint8_t> buf = memory->safeRead(ptrAddr, type.getSize());
    switch (type.getType()) {
        case TypeDecl::BOOL:
            return internal::createExpressionValue<bool>(type, buf);
        case TypeDecl::CHAR:
            return internal::createExpressionValue<int8_t>(type, buf);
        case TypeDecl::SHORT:
            return internal::createExpressionValue<int16_t>(type, buf);
        case TypeDecl::POINTER:
            return internal::createExpressionValue<uint32_t>(type, buf);
        case TypeDecl::INT:
            return internal::createExpressionValue<int32_t>(type, buf);
        case TypeDecl::LONG:
            return internal::createExpressionValue<int64_t>(type, buf);
        case TypeDecl::FLOAT:
            return internal::createExpressionValue<float>(type, buf);
        case TypeDecl::DOUBLE:
            return internal::createExpressionValue<double>(type, buf);

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

