#pragma once

#include <stdint.h>
#include <functional>

#include "Type.h"
#include "ExpressionValue.h"


namespace cish
{

namespace vm { class ExecutionContext; }

namespace ast
{


class AstNode { };

class Statement: public AstNode { };



class Expression: public AstNode
{
public:
    virtual TypeDecl getType() const = 0;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) = 0;
};


class LiteralExpression: public Expression
{
public:
    LiteralExpression(const std::string &literal);
    LiteralExpression(ExpressionValue value);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) override;

private:
    ExpressionValue _value;
};




}
}
