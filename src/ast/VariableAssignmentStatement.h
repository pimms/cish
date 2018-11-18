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
                                Lvalue *lvalue,
                                Expression *value,
                                ConstAwareness constAwareness = ConstAwareness::STRICT);

    virtual ~VariableAssignmentStatement();
    virtual void execute(vm::ExecutionContext *context) const override;

    void executeAssignment(vm::ExecutionContext *context) const;

private:
    Lvalue *_lvalue;
    Expression *_expression;
};


}
}
