#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"

namespace cish
{
namespace ast
{


class DerefExpression: public Expression
{
public:
    DerefExpression(DeclarationContext *context, const std::string &varName);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    TypeDecl getType() const override;

private:
    VarDeclaration _varDecl;
};


}
}
