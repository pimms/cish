#pragma once

#include <stdint.h>
#include <functional>
#include <memory>
#include <stack>

#include "Type.h"
#include "ExpressionValue.h"

#include "../vm/Variable.h"


namespace cish::vm
{
class ExecutionContext;
}

namespace cish::ast
{

DECLARE_EXCEPTION(VariableNotDeclaredException);
DECLARE_EXCEPTION(VariableNotDefinedException);
DECLARE_EXCEPTION(InvalidTypeException);
DECLARE_EXCEPTION(InvalidCastException);
DECLARE_EXCEPTION(InvalidStatementException);
DECLARE_EXCEPTION(InvalidOperationException);


class AstNode {
public:
    typedef std::shared_ptr<AstNode> Ptr;

    virtual ~AstNode() {};
};


class Statement: public AstNode
{
public:
    typedef std::shared_ptr<Statement> Ptr;

    virtual ~Statement();

    void execute(vm::ExecutionContext*) const;
    vm::Variable* allocateEphemeral(vm::ExecutionContext *context, TypeDecl type) const;

protected:
    virtual void virtualExecute(vm::ExecutionContext*) const = 0;

    /**
     * Synchronize execution with the execution thread. This method
     * may block for an undetermined amount of time.
     *
     * synchronize is in itself idempotent, but if called more than
     * once (per execute-cycle) the call(s) must be followed by
     * EXACTLY one 'desynchronize()' call.
     */
    void synchronize(vm::ExecutionContext *context) const;

    /**
     * Only call if you're ad-hoc implementing a Statement outside
     * the default Statement::execute method.
     */
    void desynchronize(vm::ExecutionContext *context) const;

private:

    mutable std::stack<std::vector<std::unique_ptr<vm::Variable>>> _ephemeralVariables;

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
    virtual void virtualExecute(vm::ExecutionContext*) const override;
};

}
