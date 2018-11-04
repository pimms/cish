#pragma once

#include <stdint.h>
#include <functional>

#include "Type.h"
#include "ExpressionValue.h"
#include "DeclarationContext.h"


namespace cish
{
namespace vm { class ExecutionContext; }
namespace ast
{


DECLARE_EXCEPTION(VariableNotDeclaredException);
DECLARE_EXCEPTION(VariableNotDefinedException);
DECLARE_EXCEPTION(InvalidTypeException);
DECLARE_EXCEPTION(InvalidCastException);


class AstNode {
public:
    virtual ~AstNode() {};
};



class Statement: public AstNode
{
public:
    virtual ~Statement() {};
    virtual void execute(vm::ExecutionContext*) = 0;
};

class Expression: public AstNode
{
public:
    virtual ~Expression() {};
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
