#pragma once

#include "Lvalue.h"
#include "StructLayout.h"

namespace cish::ast
{

DECLARE_EXCEPTION(InvalidAccessException);

class StructAccessExpression : public Lvalue
{
public:
    enum AccessType
    {
        OBJECT = 15,    // a.b
        POINTER,        // a->b
    };

    StructAccessExpression(Expression::Ptr structExpr,
                           const std::string &memberName,
                           AccessType accessType);

    virtual TypeDecl getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _expression;

    const StructLayout *_struct;
    const StructField *_field;
};

}
