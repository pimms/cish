#pragma once

#include "AstNodes.h"

#include <vector>
#include <memory>

namespace cish
{
namespace ast
{

typedef std::vector<Statement::Ptr> StatementList;

class SuperStatement: public Statement
{
public:
    typedef std::shared_ptr<SuperStatement> Ptr;

    virtual ~SuperStatement() = default;

    const StatementList& getStatements() const;
    void addStatement(Statement::Ptr statement);

    virtual void virtualExecute(vm::ExecutionContext*) const override;
    void executeChildStatements(vm::ExecutionContext*) const;

private:
    StatementList _statements;
};


}
}
