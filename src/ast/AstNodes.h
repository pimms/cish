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

class SuperStatement;


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
    Statement(SuperStatement *super);
    virtual ~Statement() {};
    virtual void execute(vm::ExecutionContext*) const = 0;

    SuperStatement* getSuperStatement() const;

private:
    SuperStatement *_superStatement;
};

class Expression: public AstNode
{
public:
    virtual ~Expression() {};
    virtual TypeDecl getType() const = 0;
    virtual ExpressionValue evaluate(vm::ExecutionContext*) const = 0;
};


}
}
