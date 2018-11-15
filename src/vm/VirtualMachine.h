#pragma once

#include <stdint.h>
#include "../ast/Ast.h"
#include "../Exception.h"

namespace cish
{
namespace vm
{

class Memory;
class Executor;

DECLARE_EXCEPTION(VmException);

struct VmOptions
{
    // The total size of the memory in bytes
    uint32_t heapSize;

    // The smallest supported allocation unit, in bytes. Any allocation
    // attempt of a lower amount of bytes will result in al allocation
    // of 'minAllocSize' bytes.
    uint32_t minAllocSize;
};


class VirtualMachine
{
public:
    VirtualMachine(const VmOptions &opts, ast::Ast::Ptr ast);
    ~VirtualMachine();

    const ExecutionContext* getExecutionContext() const;

    void executeNextStatement();

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
};


}
}
