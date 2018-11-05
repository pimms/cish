#include "VariableDeclarationStatement.h"

#include "VariableAssignmentStatement.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Variable.h"


namespace cish
{
namespace ast
{

VariableDeclarationStatement::VariableDeclarationStatement(
        DeclarationContext *declContext,
        TypeDecl type,
        const std::string &varName)
    : VariableDeclarationStatement(declContext, type, varName, nullptr) { }

VariableDeclarationStatement::VariableDeclarationStatement(
        DeclarationContext *declContext,
        TypeDecl type,
        const std::string &varName,
        Expression *value):
    _type(type),
    _varName(varName),
    _assignment(nullptr)
{
    declContext->declareVariable(type, _varName);

    if (value != nullptr) {
        _assignment = new VariableAssignmentStatement(declContext, _varName, value);
    }
}

VariableDeclarationStatement::~VariableDeclarationStatement()
{
    if (_assignment) {
        delete _assignment;
    }
}

void VariableDeclarationStatement::execute(vm::ExecutionContext *context) const
{
    vm::Allocation::Ptr alloc = context->getMemory()->allocate(_type.getSize());
    vm::Variable *var = new vm::Variable(_type, std::move(alloc));

    context->getStackFrame()->addVariable(_varName, var);

    if (_assignment != nullptr) {
        _assignment->execute(context);
    }
}


}
}

