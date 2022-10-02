#pragma once

#include "AstNodes.h"

namespace cish::ast
{

class TypeCastExpression : public Expression
{
public:
    TypeCastExpression(TypeDecl type, Expression::Ptr expr);

    TypeDecl getType() const override;
    ExpressionValue evaluate(vm::ExecutionContext *ctx) const override;

private:
    TypeDecl _type;
    Expression::Ptr _expression;
};

}
