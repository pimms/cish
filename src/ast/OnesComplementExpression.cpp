#include "OnesComplementExpression.h"

namespace cish
{
namespace ast
{


OnesComplementExpression::OnesComplementExpression(Expression::Ptr expression):
    _expression(expression)
{
    const TypeDecl type = expression->getType();
    if (type == TypeDecl::POINTER || type == TypeDecl::FLOAT || type == TypeDecl::DOUBLE) {
        Throw(InvalidOperationException, "Cannot use the '~'-operator on type %s", type.getName());
    }
}

ExpressionValue OnesComplementExpression::evaluate(vm::ExecutionContext *context) const
{
    const int32_t val = _expression->evaluate(context).get<int32_t>();
    const int32_t flipped = ~val;
    return ExpressionValue(TypeDecl::INT, flipped);
}

TypeDecl OnesComplementExpression::getType() const
{
    /*
     * From what I quickly researched on the '~' operator, it seems like it
     * always evaluates to 'int'. Which is fine I guess.
     * I've no idea if this is correct. This outputs '0xffffefef
     *
     * #include <stdio.h>
     * int main() {
     *     short n = 0x1010;
     *     printf("negated: %x\n", ~n);
     *     return 0;
     * }
     */
    return TypeDecl::INT;
}



}
}
