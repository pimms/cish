#pragma once

#include "Scope.h"
#include "Memory.h"
#include "ExecutionThread.h"
#include "Callable.h"

#include "../Exception.h"

#include "../ast/ExpressionValue.h"
#include "../ast/StringTable.h"

#include <vector>
#include <iostream>


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
     * Copies all stirngs from the StringTable into the Memory-memoryspace
     * and makes the strings resolvable. This method should be called exactly
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
    void returnCurrentFunction(ast::ExpressionValue retval);
    bool currentFunctionHasReturned() const;
    ast::ExpressionValue getCurrentFunctionReturnValue() const;

    Scope* getScope() const;
    Memory* getMemory() const;

    virtual void yieldOnStatement(const ast::Statement *statement);
    virtual const Callable::Ptr getFunctionDefinition(const std::string &funcName) const;

    std::ostream* getStdout();

private:
    struct FunctionFrame
    {
        std::vector<Scope*> scopes;
        bool hasReturned;
        ast::ExpressionValue returnValue;
    };

    Scope *_globalScope;
    std::vector<FunctionFrame> _frameStack;

    Memory *_memory;
    std::map<ast::StringId, Allocation::Ptr> _stringMap;

    std::ostream *_stdout;
};


}
}
