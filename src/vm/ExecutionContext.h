#pragma once

#include "Scope.h"
#include "Memory.h"
#include "ExecutionThread.h"
#include "Callable.h"
#include "IStream.h"

#include "../Exception.h"

#include "../ast/ExpressionValue.h"
#include "../ast/StringTable.h"

#include <vector>
#include <iostream>
#include <stack>


namespace cish
{
namespace ast
{

class Statement;

}

namespace vm
{


DECLARE_EXCEPTION(StackUnderflowException);
DECLARE_EXCEPTION(StackOverflowException);
DECLARE_EXCEPTION(NoEntryPointException);


class ExecutionContext
{
public:
    ExecutionContext(Memory *memory);
    virtual ~ExecutionContext();

    /**
     * Copies all strings from the StringTable into the Memory-memoryspace
     * and makes the strings resolvable. This method must be called exactly
     * once.
     */
    void copyStringTable(const ast::StringTable *stringTable);

    /**
     * Resolve a StringId into its first allocated memory address. If the
     * StringId is not defined, a NULL-MemoryView is returned.
     */
    MemoryView resolveString(ast::StringId stringId) const;

    void pushScope();
    void popScope();

    void pushFunctionFrame();
    void popFunctionFrame();
    void setFunctionReturnBuffer(vm::Variable *buffer);
    void returnCurrentFunction(ast::ExpressionValue retval);
    bool currentFunctionHasReturned() const;
    ast::ExpressionValue getCurrentFunctionReturnValue() const;
    vm::Variable* getCurrentFunctionReturnBuffer() const;
    const ast::Statement* getCurrentStatement() const;

    Scope* getScope() const;
    Memory* getMemory() const;

    virtual void onStatementEnter(const ast::Statement *statement);
    virtual void onStatementExit(const ast::Statement *statement);
    virtual const Callable::Ptr getFunctionDefinition(const std::string &funcName) const;

    void setStdout(IStream *stream);
    IStream* getStdout();

private:
    struct FunctionFrame
    {
        std::vector<Scope*> scopes;
        bool hasReturned;
        ast::ExpressionValue returnValue;
        Variable *returnBuffer;
    };

    Scope *_globalScope;
    std::vector<FunctionFrame> _frameStack;

    std::stack<const ast::Statement*> _statementStack;

    Memory *_memory;
    std::map<ast::StringId, Allocation::Ptr> _stringMap;

    IStream *_customStdout;
    IStream *_defaultStdout;
};


}
}
