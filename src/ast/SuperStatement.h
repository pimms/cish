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

protected:
    void addStatement(Statement *statement);

private:
    StatementList _statements;
};


}
}
