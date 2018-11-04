#pragma once

#include "AstNodes.h"

#include <memory>
#include <vector>


namespace cish
{
namespace ast
{


class Ast
{
public:
    typedef std::unique_ptr<Ast> Ptr;

    Ast();
    ~Ast();

    void addRootStatement(Statement *statement);
    const std::vector<Statement*>& getRootStatements() const;

private:
    std::vector<Statement*> _rootStatements;
};


}
}
