#include "FunctionCallExpression.h"
#include "FunctionDefinition.h"
#include "../vm/ExecutionContext.h"

namespace cish
{
namespace ast
{


FunctionCallExpression::FunctionCallExpression(DeclarationContext *context,
                                               const std::string &funName,
                                               std::vector<Expression*> params):
    _params(params)
{
    // First off, make sure that the function is actually declared
    const FuncDeclaration *decl = context->getFunctionDeclaration(funName);
    if (decl == nullptr) {
        Throw(FunctionNotDeclaredException, "Function '%s' has not been declared in this scope", funName.c_str());
    }

    _funcDecl = (*decl);
    _params = params;

    verifyParameterTypes();
}

TypeDecl FunctionCallExpression::getType() const
{
    return _funcDecl.returnType;
}

ExpressionValue FunctionCallExpression::evaluate(vm::ExecutionContext *context) const
{
    const vm::Callable::Ptr funcDef = context->getFunctionDefinition(_funcDecl.name);

    std::vector<ExpressionValue> params;
    for (Expression *expr: _params) {
        params.push_back(expr->evaluate(context));
    }

    return funcDef->execute(context, params);
}

void FunctionCallExpression::verifyParameterTypes()
{
    if (!_funcDecl.varargs) {
        if (_funcDecl.params.size() != _params.size()) {
            Throw(InvalidParameterException, "Function '%s' expects %d parameters, but %d was given",
                    _funcDecl.name.c_str(), (int)_funcDecl.params.size(), (int)_params.size());
        }
    } else {
        if (_funcDecl.params.size() > _params.size()) {
            Throw(InvalidParameterException, "Function '%s' expects at least %d parameters, but only %d was given",
                    _funcDecl.name.c_str(), (int)_funcDecl.params.size(), (int)_params.size());
        }
    }

    // Important notice: iterate through the explicit parameters of the function declaration,
    // that way we can use the same loop to check vararg and non-vararg functions.
    for (int i=0; i<(int)_funcDecl.params.size(); i++) {
        if (!_params[i]->getType().castableTo(_funcDecl.params[i].type)) {
            Throw(InvalidParameterException, "Parameter %d ('%s') to function '%s' is not convertible to type '%s'",
                    i+1,
                    _funcDecl.params[i].type.getName(),
                    _funcDecl.name.c_str(),
                    _funcDecl.params[i].type.getName());
        }
    }
}


}
}
