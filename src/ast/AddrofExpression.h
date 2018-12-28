#pragma once

#include "AstNodes.h"
#include "Lvalue.h"


namespace cish
{
namespace ast
{


class AddrofExpression: public Expression
{
public:
    AddrofExpression(Lvalue::Ptr lvalue);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    TypeDecl getType() const override;

private:
    Lvalue::Ptr _lvalue;
    TypeDecl _type;
};


}
}
