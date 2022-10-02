#pragma once

#include "AstNodes.h"
#include "VariableAssignmentStatement.h"


namespace cish::vm
{
class ExecutionContext;
}

namespace cish::ast
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

    const TypeDecl& getDeclaredType() const;

protected:
    virtual void virtualExecute(vm::ExecutionContext *context) const override;

private:
    const TypeDecl _type;
    const std::string _varName;
    VariableAssignmentStatement::Ptr _assignment;
};

}
