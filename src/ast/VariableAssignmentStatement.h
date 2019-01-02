#pragma once

#include "AstNodes.h"
#include "Lvalue.h"

namespace cish
{
namespace vm { class ExecutionContext; };
namespace ast
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
    virtual void execute(vm::ExecutionContext *context) const override;

    void executeAssignment(vm::ExecutionContext *context) const;

private:
    Lvalue::Ptr _lvalue;
    Expression::Ptr _expression;


    void handleStructAssignment(vm::ExecutionContext *execContext, 
                                vm::MemoryView &dest, 
                                const ExpressionValue &source) const;
};


}
}
