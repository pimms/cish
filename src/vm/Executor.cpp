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

const Callable* Executor::getFunctionDefinition(const std::string &funcName) const
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

    const ast::FunctionDefinition *main = _ast->getFunctionDefinition("main");
    if (!main) {
        Throw(NoEntryPointException, "Entrypoint 'main' not found");
    }

    for (const ast::Statement *statement: _ast->getRootStatements()) {
        printf("Executing root statement\n");
        statement->execute(this);
    }

    printf("exec main\n");
    _exitStatus = main->execute(this, {});
    printf("main returned\n");
    _hasTerminated = true;
}


}
}
