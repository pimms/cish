#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"
#include "../vm/Callable.h"


namespace cish::vm
{
class Memory;
class Variable;
class Allocation;
}

namespace cish::ast
{
class DeclarationContext;

class FunctionDefinition: public SuperStatement, public vm::Callable
{
public:
    typedef std::shared_ptr<FunctionDefinition> Ptr;

    FunctionDefinition(DeclarationContext *context, FuncDeclaration decl);
    virtual ~FunctionDefinition() = default;

    const FuncDeclaration* getDeclaration() const override;

    ExpressionValue execute(vm::ExecutionContext *context,
                            const std::vector<ExpressionValue>& params,
                            vm::Variable *returnBuffer) const override;

protected:
    void virtualExecute(vm::ExecutionContext*) const override;

private:
    vm::Variable* convertToVariable(vm::Memory *memory, const TypeDecl &targetType, const ExpressionValue &sourceValue) const;
    void copyStruct(vm::Memory *memory, vm::Allocation *target, const TypeDecl &targetType, const ExpressionValue &sourceValue) const;

    FuncDeclaration _decl;
};

}
