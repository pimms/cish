#pragma once

#include <vector>

namespace cish
{
namespace ast
{

class Statement;

typedef std::vector<Statement*> StatementList;

class SuperStatement
{
public:
    SuperStatement(StatementList statements);
    virtual ~SuperStatement();

    const StatementList& getStatements() const;

private:
    StatementList _statements;
};


}
}
