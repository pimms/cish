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

    // Verify that we're passing the correct parameters to the function
    if (_funcDecl.params.size() != params.size()) {
        Throw(InvalidParameterException, "Function '%s' expects %d parameters, but %d was given",
                _funcDecl.name.c_str(), (int)_funcDecl.params.size(), (int)params.size());
    }

    for (int i=0; i<(int)params.size(); i++) {
        if (!params[i]->getType().castableTo(_funcDecl.params[i].type)) {
            Throw(InvalidParameterException, "Parameter %d ('%s') to function '%s' is not convertible to type '%s'",
                    i+1,
                    _funcDecl.params[i].type.getName(),
                    _funcDecl.name.c_str(),
                    _funcDecl.params[i].type.getName());
        }
    }
}

TypeDecl FunctionCallExpression::getType() const
{
    return _funcDecl.returnType;
}

ExpressionValue FunctionCallExpression::evaluate(vm::ExecutionContext *context) const
{
    const FunctionDefinition *funcDef = context->getFunctionDefinition(_funcDecl.name);

    std::vector<ExpressionValue> params;
    for (Expression *expr: _params) {
        params.push_back(expr->evaluate(context));
    }

    funcDef->execute(context, params);

    Throw(Exception, "NOT IMPLEMENTED - How do we handle return types, lol");
}

}
}
