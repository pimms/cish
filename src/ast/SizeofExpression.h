#pragma once

#include "AstNodes.h"

namespace cish::ast
{

class SizeofExpression : public Expression
{
public:
    SizeofExpression(Expression::Ptr expression);
    SizeofExpression(TypeDecl type);

    TypeDecl getType() const override;
    ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    uint32_t _size;
};

}
