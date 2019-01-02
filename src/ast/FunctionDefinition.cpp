#include "FunctionDefinition.h"

#include "DeclarationContext.h"
#include "FunctionCallExpression.h"

#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


FunctionDefinition::FunctionDefinition(DeclarationContext *context,
                                       FuncDeclaration decl):
    _decl(decl)
{
    context->declareFunction(decl);
}

const FuncDeclaration* FunctionDefinition::getDeclaration() const
{
    return &_decl;
}

ExpressionValue FunctionDefinition::execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const
{
    synchronize(context);

    if (params.size() != _decl.params.size()) {
        Throw(InvalidParameterException, "Function '%s' expected %d params, got %d",
                _decl.name.c_str(), _decl.params.size(), params.size());
    }

    context->pushFunctionFrame();
    vm::Memory *memory = context->getMemory();
    vm::Scope *scope = context->getScope();

    for (int i=0; i<params.size(); i++) {
        if (!params[i].getIntrinsicType().castableTo(_decl.params[i].type)) {
            Throw(InvalidParameterException, "Value of type '%s' passed to parameter '%s' of "
                                             "function '%s' is not convertible to expected type '%s'",
                params[i].getIntrinsicType().getName(),
                _decl.params[i].name.c_str(),
                _decl.name.c_str(),
                _decl.params[i].type.getName());
        }

        const std::string varName = _decl.params[i].name;
        if (varName != "") {
            vm::Variable *var = convertToVariable(memory, _decl.params[i].type, params[i]);
            scope->addVariable(varName, var);
        }
    }

    executeChildStatements(context);
    ExpressionValue retVal = context->getCurrentFunctionReturnValue();
    context->popFunctionFrame();
    return retVal;
}

void FunctionDefinition::virtualExecute(vm::ExecutionContext*) const
{
    Throw(Exception, "FunctionDefinition::virtualExecute should never be called");
}

vm::Variable* FunctionDefinition::convertToVariable(vm::Memory *memory,
                                                    const TypeDecl &targetType,
                                                    const ExpressionValue &sourceValue) const
{
    TypeDecl sourceType = sourceValue.getIntrinsicType();
    vm::Allocation::Ptr alloc = memory->allocate(targetType.getSize());

    switch (targetType.getType()) {
        case TypeDecl::BOOL:
            alloc->write<bool>(sourceValue.get<bool>());
            break;
        case TypeDecl::CHAR:
            alloc->write<char>(sourceValue.get<char>());
            break;
        case TypeDecl::SHORT:
            alloc->write<short>(sourceValue.get<short>());
            break;
        case TypeDecl::INT:
            alloc->write<int>(sourceValue.get<int>());
            break;
        case TypeDecl::FLOAT:
            alloc->write<float>(sourceValue.get<float>());
            break;
        case TypeDecl::DOUBLE:
            alloc->write<double>(sourceValue.get<double>());
            break;
        case TypeDecl::POINTER:
            alloc->write<uint32_t>(sourceValue.get<uint32_t>());
            break;
        case TypeDecl::STRUCT:
            copyStruct(memory, alloc.get(), targetType, sourceValue);
            break;
        default:
            Throw(InvalidTypeException, "Unable to allocate variable of type '%s' for function '%s'",
                    targetType.getName(), _decl.name.c_str());
    }

    return new vm::Variable(sourceType, std::move(alloc));
}

void FunctionDefinition::copyStruct(vm::Memory *memory,
                                    vm::Allocation *target,
                                    const TypeDecl &targetType,
                                    const ExpressionValue &sourceValue) const
{
    const uint32_t sourceBaseAddr = sourceValue.get<uint32_t>();
    const uint32_t size = targetType.getSize();
    
    const vm::MemoryView sourceView = memory->getView(sourceBaseAddr);
    const uint8_t *sourceBuf = sourceView.readBuf(size);

    target->writeBuf(sourceBuf, size);
}


}
}
