#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"


namespace cish
{
namespace ast
{


class AddrofExpression: public Expression
{
public:
    AddrofExpression(DeclarationContext *context, const std::string &varName);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    TypeDecl getType() const override;

private:
    VarDeclaration _varDecl;
};


}
}
