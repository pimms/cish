#include "SuperStatement.h"

#include "AstNodes.h"

#include "../vm/ExecutionContext.h"

namespace cish::ast
{

/*
==============
SuperStatement
==============
*/
const StatementList& SuperStatement::getStatements() const
{
    return _statements;
}

void SuperStatement::addStatement(Statement::Ptr statement)
{
    _statements.push_back(statement);
}

void SuperStatement::virtualExecute(vm::ExecutionContext*) const
{
    Throw(Exception, "SuperStatement::execute should never be called"); 
}

void SuperStatement::executeChildStatements(vm::ExecutionContext *context) const
{
    if (context->currentFunctionHasReturned())
        return;

    for (const Statement::Ptr& statement: _statements) {
        statement->execute(context);
    }
}

}
