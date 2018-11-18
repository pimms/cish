#include "Executor.h"

#include "../ast/SuperStatement.h"
#include "../Exception.h"


namespace cish
{
namespace vm
{


Executor::Executor(Memory *memory, ast::Ast::Ptr ast):
    ExecutionContext(memory),
    _ast(std::move(ast)),
    _exitStatus(-1337),
    _hasTerminated(false)
{

}

Executor::~Executor()
{

}

void Executor::yieldOnStatement(const ast::Statement *statement)
{
    await();
}

const Callable::Ptr Executor::getFunctionDefinition(const std::string &funcName) const
{
    return _ast->getFunctionDefinition(funcName);
}

ast::ExpressionValue Executor::getExitStatus() const
{
    if (!_hasTerminated) {
        Throw(Exception, "The executing program has not terminated, exit status unavailable");
    }

    return _exitStatus;
}

void Executor::execute()
{
    await();
    copyStringTable(_ast->getStringTable());

    const Callable::Ptr main = _ast->getFunctionDefinition("main");
    if (!main) {
        Throw(NoEntryPointException, "Entrypoint 'main' not found");
    }

    for (const ast::Statement *statement: _ast->getRootStatements()) {
        statement->execute(this);
    }

    _exitStatus = main->execute(this, {});
    _hasTerminated = true;
}


}
}
