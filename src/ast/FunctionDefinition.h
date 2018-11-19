#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"
#include "../vm/Callable.h"


namespace cish
{
namespace vm { class Memory; class Variable; }
namespace ast
{


class DeclarationContext;

class FunctionDefinition: public SuperStatement, public vm::Callable
{
public:
    typedef std::shared_ptr<FunctionDefinition> Ptr;

    FunctionDefinition(DeclarationContext *context, FuncDeclaration decl);
    virtual ~FunctionDefinition() = default;

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
