#pragma once

#include "AstNodes.h"
#include "StringTable.h"


namespace cish
{
namespace ast
{


class StringLiteralExpression: public Expression
{
public:
    StringLiteralExpression(StringId stringId);

    ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    TypeDecl getType() const override;

private:
    StringId _stringId;
    TypeDecl _type;
};


}
}