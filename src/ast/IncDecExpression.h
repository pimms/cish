#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"


namespace cish
{
namespace ast
{

class VariableReferenceExpression;


class UnaryExpression: public Expression
{
public:
    enum Operation
    {
        INC_PRE,
        INC_POST,
        DEC_PRE,
        DEC_POST,

        ADDROF,
        SIZEOF,
        DEREFERENCE,
        NOT,
    };

    UnaryExpression(Operation operation, Expression *expression);
    UnaryExpression(Operation operation, VariableReferenceExpression *expression);
    ~UnaryExpression();

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    Operation _operation;
    Expression *_expression;
};


class BinaryExpression;

class IncDecExpression: public Expression
{
public:
    enum Operation {
        PREFIX_INCREMENT,
        POSTFIX_INCREMENT,
        PREFIX_DECREMENT,
        POSTFIX_DECREMENT,
    };

    IncDecExpression(DeclarationContext *context, Operation type, const std::string &varName);
    ~IncDecExpression();

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    int getMutationValue() const;

    Operation _operation;
    VarDeclaration _varDecl;
};


}
}
