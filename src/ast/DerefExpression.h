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
    DerefExpression(DeclarationContext *context, Expression *expr);
   ~DerefExpression();

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    TypeDecl getType() const override;

private:
    Expression *_expression;
};


}
}
