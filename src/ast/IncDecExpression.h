#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"
#include "Lvalue.h"


namespace cish
{
namespace ast
{


class IncDecExpression: public Expression
{
public:
    enum Operation {
        PREFIX_INCREMENT,
        POSTFIX_INCREMENT,
        PREFIX_DECREMENT,
        POSTFIX_DECREMENT,
    };

    IncDecExpression(DeclarationContext *context, Operation type, Lvalue *lvalue);
    ~IncDecExpression();

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

private:
    int getMutationValue() const;

    Operation _operation;
    Lvalue *_lvalue;
};


}
}
