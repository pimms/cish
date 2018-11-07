#pragma once

#include "ExecutionContext.h"
#include "../ast/Ast.h"


namespace cish
{
namespace vm
{

class Memory;


class Executor: public ExecutionContext, public ExecutionThread
{
public:
    Executor(Memory *memory, ast::Ast::Ptr ast);
    ~Executor();

    // From ExecutionContext
    virtual void yieldOnStatement(const ast::Statement *statement) override;

protected:
    // From ExecutionThread
    virtual void execute() override;

private:
    ast::Ast::Ptr _ast;
};


}
}
