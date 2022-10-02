#pragma once

#include "AstNodes.h"
#include "Lvalue.h"

namespace cish::vm
{
class ExecutionContext;
}

namespace cish::ast
{

class DeclarationContext;

class VariableAssignmentStatement: public Statement
{
public:
    /**
     * Exists so VariableDeclarationStatement is able to piggy-back on
     * VariableAssignmentStatement to initialize constant variables.
     */
    enum class ConstAwareness
    {
        IGNORE,
        STRICT,
    };

    VariableAssignmentStatement(DeclarationContext *context,
                                Lvalue::Ptr lvalue,
                                Expression::Ptr value,
                                ConstAwareness constAwareness = ConstAwareness::STRICT);

    virtual ~VariableAssignmentStatement() = default;

    void executeAssignment(vm::ExecutionContext *context) const;

protected:
    virtual void virtualExecute(vm::ExecutionContext *context) const override;

private:
    Lvalue::Ptr _lvalue;
    Expression::Ptr _expression;


    void handleStructAssignment(vm::ExecutionContext *execContext, 
                                vm::MemoryView &dest, 
                                const ExpressionValue &source) const;
};

}
