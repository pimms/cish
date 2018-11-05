#pragma once

#include <stdint.h>

#include "../ast/Ast.h"
#include "../ast/SuperStatement.h"

#include "Memory.h"
#include "ExecutionContext.h"

namespace cish
{
namespace vm
{

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

private:
    void executeNextGlobalStatement();
    void executeStatement(const ast::Statement *statement);

    Memory *_memory;
    ExecutionContext *_executionContext;
    ast::Ast::Ptr _ast;

    bool _globalInitComplete;
    int _globalStatementIdx;
    std::vector<const ast::SuperStatement*> _superStack;
};


}
}
