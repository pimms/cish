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
    virtual ~SuperStatement();

    const StatementList& getStatements() const;
    void addStatement(Statement *statement);

private:
    StatementList _statements;
};


}
}
