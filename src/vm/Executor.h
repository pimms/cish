#pragma once

#include "ExecutionContext.h"
#include "../ast/Ast.h"

#include <vector>
#include <string>

namespace cish::vm
{

class Memory;

class Executor: public ExecutionContext, public ExecutionThread
{
public:
    Executor(Memory *memory, ast::Ast::Ptr ast);
    ~Executor();

    void setCliArgs(const std::vector<ast::ExpressionValue> &args);

    // From ExecutionContext
    virtual void onStatementEnter(const ast::Statement *statement) override;
    virtual const Callable::Ptr getFunctionDefinition(const std::string &funcName) const override;

    ast::ExpressionValue getExitStatus() const;

protected:
    // From ExecutionThread
    virtual void execute() override;

private:
    std::vector<ast::ExpressionValue> prepareMainArguments(const Callable::Ptr main) const;

    ast::Ast::Ptr _ast;
    ast::ExpressionValue _exitStatus;
    std::vector<ast::ExpressionValue> _cliArgs;
    bool _hasTerminated;
};

}
