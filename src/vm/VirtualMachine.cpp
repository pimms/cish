#include "VirtualMachine.h"
#include "Executor.h"
#include "Memory.h"
#include "../Exception.h"

using cish::ast::Ast;

namespace cish::vm
{

DECLARE_EXCEPTION(CommandLineArgumentException)

VirtualMachine::VirtualMachine(const VmOptions &opts, Ast::Ptr ast):
    _memory(new Memory(opts.heapSize, opts.minAllocSize)),
    _executor(new Executor(_memory, ast)),
    _started(false)
{
    auto args = prepareCliArguments(opts.args);
    _executor->setCliArgs(args);
}

VirtualMachine::~VirtualMachine()
{
    _argvElements.clear();
    _argvBuffer = nullptr;

    _executor->terminate();
    delete _executor;
    delete _memory;
}

ExecutionContext* VirtualMachine::getExecutionContext() const
{
    return _executor;
}

void VirtualMachine::executeBlocking()
{
    if (_started) {
        Throw(VmException, "Cannot restart a started VM");
    }

    _started = true;
    _executor->setWaitForResume(false);
    _executor->runBlocking();
}

void VirtualMachine::startSync()
{
    if (_started) {
        Throw(VmException, "Cannot restart a started VM");
    }

    _started = true;
    _executor->setWaitForResume(true);
    _executor->startAsync();
}

void VirtualMachine::executeNextStatement()
{
    if (_started) {
        if (_executor->isRunning()) {
            _executor->cycle();
        } else {
            Throw(VmException, "Virtual machine not running");
        }
    } else {
        Throw(VmException,
            "Virtual machine not started "
            "(did you forget to call startSync()?)");
    }
}

bool VirtualMachine::hasStarted() const
{
    return _started;
}

bool VirtualMachine::isRunning() const
{
    return _executor->isRunning();
}

int VirtualMachine::getExitCode() const
{
    return _executor->getExitStatus().get<int>();
}

std::shared_ptr<Exception> VirtualMachine::getRuntimeError() const
{
    return _executor->getRuntimeError();
}

void VirtualMachine::terminate()
{
    _executor->terminate();
}

std::vector<ast::ExpressionValue> VirtualMachine::prepareCliArguments(std::vector<std::string> args)
{
    const Callable::Ptr main = _executor->getFunctionDefinition("main");
    if (!main) {
        Throw(Exception, "Failed to find definition for function 'main'");
        return {};
    }

    const std::vector<ast::VarDeclaration> mainParams = main->getDeclaration()->params;
    std::vector<ast::ExpressionValue> result;

    // argc
    if (mainParams.size() >= 1) {
        ast::TypeDecl paramType = mainParams[0].type;
        if (paramType != ast::TypeDecl::INT) {
            Throw(CommandLineArgumentException, "Param 1 must be of type 'int'");
            return {};
        }

        ast::ExpressionValue argcValue = ast::ExpressionValue(ast::TypeDecl::INT, (int)args.size());
        result.push_back(argcValue);
    }

    // argv
    if (mainParams.size() >= 2 && args.size() > 0) {
        const ast::TypeDecl stringType = ast::TypeDecl::getPointer(ast::TypeDecl::CHAR);
        const ast::TypeDecl stringArrayType = ast::TypeDecl::getPointer(stringType);
        const ast::TypeDecl paramType = mainParams[1].type;
        if (paramType != stringArrayType) {
            Throw(CommandLineArgumentException, "Param 2 must be of type 'char**'");
            return {};
        }

        // Allocate the argv array.
        _argvBuffer = _memory->allocate(args.size() * stringType.getSize());

        for (int i=0; i<(int)args.size(); i++) {
            Allocation::Ptr vmArg = _memory->allocate(args[i].size() + 1);
            vmArg->writeBuf(args[i].data(), args[i].size(), 0);
            vmArg->write<uint8_t>(0, args[i].size());
            _argvBuffer->write<uint32_t>(vmArg->getAddress(), sizeof(uint32_t) * i);

            _argvElements.push_back(std::move(vmArg));
        }

        ast::ExpressionValue argvValue = ast::ExpressionValue(stringArrayType, _argvBuffer->getAddress());
        result.push_back(argvValue);
    }

    // Just null out the rest of the parameters
    for (int i=result.size(); i<mainParams.size(); i++) {
        result.push_back(ast::ExpressionValue(mainParams[i].type, 0));
    }

    return result;
}

}
