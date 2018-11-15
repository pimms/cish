#include "VariableDeclarationStatement.h"
#include "DeclarationContext.h"
#include "VariableAssignmentStatement.h"

#include "../vm/ExecutionContext.h"
#include "../vm/Variable.h"


namespace cish
{
namespace ast
{

VariableDeclarationStatement::VariableDeclarationStatement(
        DeclarationContext *context,
        TypeDecl type,
        const std::string &varName,
        Expression *value):
    _type(type),
    _varName(varName),
    _assignment(nullptr)
{
    context->declareVariable(type, _varName);

    if (value != nullptr) {
        VariableReference *varRef = new VariableReference(context, varName);

        auto constAwareness = VariableAssignmentStatement::ConstAwareness::IGNORE;
        _assignment = new VariableAssignmentStatement(context, varRef, value, constAwareness);
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
    if (context->currentFunctionHasReturned())
        return;

    Statement::execute(context);

    vm::Allocation::Ptr alloc = context->getMemory()->allocate(_type.getSize());
    vm::Variable *var = new vm::Variable(_type, std::move(alloc));

    context->getScope()->addVariable(_varName, var);

    if (_assignment != nullptr) {
        _assignment->executeAssignment(context);
    }
}

const TypeDecl& VariableDeclarationStatement::getDeclaredType() const
{
    return _type;
}


}
}

