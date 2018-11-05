#pragma once

#include "AstNodes.h"
#include "../Exception.h"

namespace cish
{
namespace ast
{

class DeclarationContext;

class VariableReferenceExpression: public Expression
{
public:
    VariableReferenceExpression(DeclarationContext *declContext, const std::string &varName);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext *ctx) const override;

private:
    std::string _name;
    TypeDecl _type;
};


}
}
