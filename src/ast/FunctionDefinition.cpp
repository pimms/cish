#include "FunctionDefinition.h"
#include "../vm/ExecutionContext.h"
#include "FunctionCallExpression.h"


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

FunctionDefinition::~FunctionDefinition()
{
}

const FuncDeclaration* FunctionDefinition::getDeclaration() const
{
    return &_decl;
}

void FunctionDefinition::execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const
{
    Statement::execute(context);

    if (params.size() != _decl.params.size()) {
        Throw(InvalidParameterException, "Function '%s' expected %d params, got %d",
                _decl.name.c_str(), _decl.params.size(), params.size());
    }

    for (int i=0; i<params.size(); i++) {
        if (params[i].getIntrinsicType().castableTo(_decl.params[i].type)) {
            Throw(InvalidParameterException, "Value of type '%s' passed to parameter '%s' of "
                                             "function '%s' is not convertible to expected type '%s'",
                params[i].getIntrinsicType().getName(),
                _decl.params[i].name.c_str(),
                _decl.name.c_str(),
                _decl.params[i].type.getName());
        }
    }

    context->pushFunctionFrame();

    // TODO: Declare them fkn variables

    context->popFunctionFrame();

}


}
}
