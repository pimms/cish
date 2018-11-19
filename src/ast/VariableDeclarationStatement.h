#pragma once

#include "AstNodes.h"
#include "VariableAssignmentStatement.h"


namespace cish
{
namespace vm { class ExecutionContext; }
namespace ast
{

class DeclarationContext;
class VariableAssignmentStatement;


class VariableDeclarationStatement: public Statement
{
public:
    VariableDeclarationStatement(
            DeclarationContext *declContext,
            TypeDecl type,
            const std::string &varName,
            Expression::Ptr value);
    virtual ~VariableDeclarationStatement() = default;
    virtual void execute(vm::ExecutionContext *context) const override;

    const TypeDecl& getDeclaredType() const;

private:
    const TypeDecl _type;
    const std::string _varName;
    VariableAssignmentStatement::Ptr _assignment;
};


}
}
