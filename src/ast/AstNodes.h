#pragma once

#include <stdint.h>
#include <functional>
#include <memory>

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


struct SourcePosition
{
    SourcePosition() { startLine = globStartChar = globEndChar = 0; }

    int startLine;
    int globStartChar;
    int globEndChar;
};

class AstNode {
public:
    typedef std::shared_ptr<AstNode> Ptr;

    virtual ~AstNode() {}

    void setSourcePosition(const SourcePosition &pos);
    SourcePosition getSourcePosition() const;

private:
    SourcePosition _sourcePos;
};



class Statement: public AstNode
{
public:
    typedef std::shared_ptr<Statement> Ptr;

    virtual ~Statement() {
        printf("~[line %d (%d - %d)] - %s\n", 
                getSourcePosition().startLine,
                getSourcePosition().globStartChar,
                getSourcePosition().globEndChar, 
                typeid(*this).name());
    };

    // 1. Always override this method
    // 2. Always call this method from subclasses
    virtual void execute(vm::ExecutionContext*) const;
};

class Expression: public AstNode
{
public:
    typedef std::shared_ptr<Expression> Ptr;

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
