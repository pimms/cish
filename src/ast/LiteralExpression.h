#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


class LiteralExpression: public Expression
{
public:
    LiteralExpression(const std::string &literal);
    LiteralExpression(ExpressionValue value);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    ExpressionValue _value;
};


}
}
