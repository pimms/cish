#pragma once

#include <stdint.h>
#include "../ast/Ast.h"
#include "../Exception.h"

namespace cish::vm
{

class Memory;
class Executor;

DECLARE_EXCEPTION(VmException);

struct VmOptions
{
    VmOptions() {
        heapSize = 1 << 10;
        minAllocSize = 4;
    }
    // The total size of the memory in bytes
    uint32_t heapSize;

    // The smallest supported allocation unit, in bytes. Any allocation
    // attempt of a lower amount of bytes will result in al allocation
    // of 'minAllocSize' bytes.
    uint32_t minAllocSize;

    std::vector<std::string> args;
};


class VirtualMachine
{
public:
    VirtualMachine(const VmOptions &opts, ast::Ast::Ptr ast);
    ~VirtualMachine();

    ExecutionContext* getExecutionContext() const;

    /**
     * Blocks until the underlying program has terminated.
     */
    void executeBlocking();

    /**
     * Starts the program, but does not execute any operations.
     * executeNextStatement must be called for each statement.
     */
    void startSync();
    void executeNextStatement();

    bool hasStarted() const;
    bool isRunning() const;
    int getExitCode() const;
    std::shared_ptr<Exception> getRuntimeError() const;

    /**
     * Termniate the VM. This method will not return until the
     * associated background thread is joined, and will not have
     * any effect if the VM already is stopped.
     */
    void terminate();

private:
    Memory *_memory;
    Executor *_executor;
    bool _started;

    // We need to hold a reference to the allocated CLI parameters.
    Allocation::Ptr _argvBuffer;
    std::vector<Allocation::Ptr> _argvElements;

    std::vector<ast::ExpressionValue> prepareCliArguments(std::vector<std::string> args);
};

}
