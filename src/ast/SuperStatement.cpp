#include "SuperStatement.h"

#include "AstNodes.h"


namespace cish
{
namespace ast
{


/*
==============
SuperStatement
==============
*/
SuperStatement::~SuperStatement()
{
    for (Statement *s: _statements) {
        delete s;
    }
}

const StatementList& SuperStatement::getStatements() const
{
    return _statements;
}

void SuperStatement::addStatement(Statement *statement)
{
    _statements.push_back(statement);
}


}
}
