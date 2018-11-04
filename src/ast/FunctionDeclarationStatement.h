#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"

namespace cish
{
namespace ast
{
    
class FunctionDeclarationStatement: public Statement
{
public:
    FunctionDeclarationStatement(DeclarationContext *context, FuncDeclaration decl);
    virtual void execute(vm::ExecutionContext*) override;
    
private:
    FuncDeclaration _decl;
};

}
}
