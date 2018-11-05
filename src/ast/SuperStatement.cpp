#include "SuperStatement.h"

#include "AstNodes.h"


namespace cish
{
namespace ast
{


SuperStatement::SuperStatement(StatementList statements):
    _statements(statements)
{ }

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


}
}
