#include "Executor.h"

#include "../ast/SuperStatement.h"
#include "../Exception.h"


namespace cish
{
namespace vm
{


Executor::Executor(Memory *memory, ast::Ast::Ptr ast):
    ExecutionContext(memory),
    _ast(std::move(ast))
{

}

Executor::~Executor()
{

}

void Executor::yieldOnStatement(const ast::Statement *statement)
{
    await();
}

void Executor::execute()
{
    const ast::SuperStatement *main = _ast->getFunctionDefinition("main");
    if (!main) {
        printf("No main func def\n");
        Throw(NoEntryPointException, "Entrypoint 'main' not found");
    }

    for (ast::Statement *statement: _ast->getRootStatements()) {
        printf("Executing root statement\n");
        statement->execute(this);
    }

    printf("exec main\n");
    main->execute(this);
}


}
}
