#pragma once

#include "AstNodes.h"


namespace cish
{
namespace ast
{


/**
 * Holds a literal value that can be changed to any other literal
 * value OF THE SAME TYPE.
 *
 * There are a lot of 'compile-time' checks going on, and they are
 * worthless if the sand can shift beneath their feet.
 */
class MutableLiteralExpression: public Expression
{
public:
    MutableLiteralExpression(const std::string &literal);
    MutableLiteralExpression(ExpressionValue value);

    virtual TypeDecl getType() const override;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const override;

    void setValue(const ExpressionValue &value);

private:
    ExpressionValue _value;
};


}
}
