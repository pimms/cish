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


DECLARE_EXCEPTION(VariableNotDeclaredException);
DECLARE_EXCEPTION(VariableNotDefinedException);
DECLARE_EXCEPTION(InvalidTypeException);
DECLARE_EXCEPTION(InvalidCastException);
DECLARE_EXCEPTION(InvalidStatementException);
DECLARE_EXCEPTION(InvalidOperationException);


class AstNode {
public:
    virtual ~AstNode() {};
};



class Statement: public AstNode
{
public:
    virtual ~Statement() {};

    // 1. Always override this method
    // 2. Always call this method from subclasses
    virtual void execute(vm::ExecutionContext*) const;
};

class Expression: public AstNode
{
public:
    virtual ~Expression() {};
    virtual TypeDecl getType() const = 0;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const = 0;
};



class NoOpStatement: public Statement
{
public:
    virtual void execute(vm::ExecutionContext*) const override;
};


}
}
