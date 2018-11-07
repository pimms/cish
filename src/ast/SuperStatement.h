#pragma once

#include "AstNodes.h"

#include <vector>

namespace cish
{
namespace ast
{

typedef std::vector<Statement*> StatementList;

class SuperStatement: public Statement
{
public:
    virtual ~SuperStatement();

    const StatementList& getStatements() const;
    void addStatement(Statement *statement);

    virtual void execute(vm::ExecutionContext*) const override;

private:
    StatementList _statements;
};


}
}
