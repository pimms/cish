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
    VariableAssignmentStatement(DeclarationContext *context,
                                const std::string &varName,
                                Expression *value);
    ~VariableAssignmentStatement();
    virtual void execute(vm::ExecutionContext *context) const override;
    void executeAssignment(vm::ExecutionContext *context) const;

private:
    const std::string _varName;
    Expression *_expression;
};


}
}
