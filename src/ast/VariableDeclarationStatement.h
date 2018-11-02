#pragma once

#include "AstNodes.h"


namespace cish
{
namespace vm { class ExecutionContext; }
namespace ast
{

class VariableAssignmentStatement;


class VariableDeclarationStatement: public Statement
{
public:
    VariableDeclarationStatement(
            DeclarationContext *declContext,
            TypeDecl type,
            const std::string &varName);
    VariableDeclarationStatement(
            DeclarationContext *declContext,
            TypeDecl type,
            const std::string &varName,
            Expression *value);
    ~VariableDeclarationStatement();
    virtual void execute(vm::ExecutionContext *context) override;

private:
    const TypeDecl _type;
    const std::string _varName;
    VariableAssignmentStatement *_assignment;
};


}
}
