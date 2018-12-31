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

    // TODO: Consider verifying that we actually return someting (if the returntype is
    // non-void), but I don't believe it's in any of the C-standards, so maybe just don't.
}

const FuncDeclaration* FunctionDefinition::getDeclaration() const
{
    return &_decl;
}

ExpressionValue FunctionDefinition::execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const
{
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
            vm::Variable *var = convertToVariable(memory, params[i]);
            scope->addVariable(varName, var);
        }
    }

    Statement::execute(context);
    SuperStatement::execute(context);
    ExpressionValue retVal = context->getCurrentFunctionReturnValue();
    context->popFunctionFrame();
    return retVal;
}

vm::Variable* FunctionDefinition::convertToVariable(vm::Memory *memory, const ExpressionValue &expr) const
{
    TypeDecl type = expr.getIntrinsicType();
    vm::Allocation::Ptr alloc = memory->allocate(type.getSize());

    switch (type.getType()) {
        case TypeDecl::BOOL:
            alloc->write<bool>(expr.get<bool>());
            break;
        case TypeDecl::CHAR:
            alloc->write<char>(expr.get<char>());
            break;
        case TypeDecl::SHORT:
            alloc->write<short>(expr.get<short>());
            break;
        case TypeDecl::INT:
            alloc->write<int>(expr.get<int>());
            break;
        case TypeDecl::FLOAT:
            alloc->write<float>(expr.get<float>());
            break;
        case TypeDecl::DOUBLE:
            alloc->write<double>(expr.get<double>());
            break;
        case TypeDecl::POINTER:
            alloc->write<uint32_t>(expr.get<uint32_t>());
            break;
        default:
            Throw(InvalidTypeException, "Unable to allocate variable of type '%s' for function '%s'",
                    type.getName(), _decl.name.c_str());
    }

    return new vm::Variable(type, std::move(alloc));
}


}
}
