#include "Executor.h"

#include "../ast/SuperStatement.h"
#include "../Exception.h"

DECLARE_EXCEPTION(CommandLineArgumentException)

namespace cish
{
namespace vm
{


Executor::Executor(Memory *memory, ast::Ast::Ptr ast, std::vector<std::string> args):
    ExecutionContext(memory),
    _ast(ast),
    _exitStatus(-1),
    _args(args),
    _hasTerminated(false)
{

}

Executor::~Executor()
{
    if (isRunning()) {
        Throw(Exception, "terminate() must be manually called before deleting Executor");
    }
}

void Executor::onStatementEnter(const ast::Statement *statement)
{
    ExecutionContext::onStatementEnter(statement);
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

    // Execute global statements
    for (const ast::Statement::Ptr statement: _ast->getRootStatements()) {
        statement->execute(this);
    }

    const auto cliParams = prepareMainArguments(main);
    _exitStatus = main->execute(this, cliParams, nullptr);
    _hasTerminated = true;
}

std::vector<ast::ExpressionValue> Executor::prepareMainArguments(const Callable::Ptr main) const
{
    const std::vector<ast::VarDeclaration> vars = main->getDeclaration()->params;
    std::vector<ast::ExpressionValue> parsedParams;

    if (vars.size() > 2) {
        Throw(CommandLineArgumentException,
              "Function main cannot take more than 2 parameters (it takes %d)",
              (int)vars.size());
    }

    if (vars.size() >= 1) {
        ast::VarDeclaration param1 = vars[0];
        if (!param1.type.isIntegral()) {
            Throw(CommandLineArgumentException,
                  "Parameter 0 of function main must be integral, was %s",
                  param1.type.getName());
        }

        const int argc = _args.size();
        parsedParams.push_back(ast::ExpressionValue(argc));

        if (vars.size() >= 2) {
            ast::VarDeclaration param2 = vars[1];

            ast::TypeDecl expectedType = ast::TypeDecl::getPointer(ast::TypeDecl::getPointer(ast::TypeDecl::CHAR));
            if (param2.type != expectedType) {
                Throw(CommandLineArgumentException,
                      "Parameter 1 of function main must be '%s', was '%s'",
                      expectedType.getName(),
                      param2.type.getName());
            }

            // TODO: Allocate the command line arguments and add them to 'parsedParams'
            // This probably needs to be done in VirtualMachine, as that is where the
            // memory pool is found. The Exceutor has no knowledge of the memory itself.
            fprintf(stderr, "TODO: Allocate the command line parameters and add them to 'parsedParams'\n");
        }
    }

    return parsedParams;
}


}
}
