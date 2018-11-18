#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"
#include "SuperStatement.h"
#include "../vm/Callable.h"


namespace cish
{
namespace vm { class Memory; class Variable; }
namespace ast
{


class FunctionDefinition: public SuperStatement, public vm::Callable
{
public:
    FunctionDefinition(DeclarationContext *context, FuncDeclaration decl);
    virtual ~FunctionDefinition();

    const FuncDeclaration* getDeclaration() const override;

    ExpressionValue execute(vm::ExecutionContext *context, const std::vector<ExpressionValue>& params) const override;

    bool hasReturned() const;
    ExpressionValue getReturnValue() const;

private:
    vm::Variable* convertToVariable(vm::Memory *memory, const ExpressionValue &expr) const;

    FuncDeclaration _decl;
};


}
}
