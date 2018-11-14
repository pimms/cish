#pragma once

#include "AstNodes.h"


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
                                const std::string &varName,
                                Expression *value);
    VariableAssignmentStatement(DeclarationContext *context,
                                const std::string &varName,
                                Expression *value,
                                ConstAwareness constAwareness);

    virtual ~VariableAssignmentStatement();
    virtual void execute(vm::ExecutionContext *context) const override;

    void executeAssignment(vm::ExecutionContext *context) const;

private:
    const std::string _varName;
    Expression *_expression;
};


}
}
