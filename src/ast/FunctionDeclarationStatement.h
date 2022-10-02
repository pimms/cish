#pragma once

#include "AstNodes.h"
#include "DeclarationContext.h"

namespace cish::ast
{

class FunctionDeclarationStatement: public Statement
{
public:
    FunctionDeclarationStatement(DeclarationContext *context, FuncDeclaration decl);

protected:
    virtual void virtualExecute(vm::ExecutionContext*) const override;

private:
    FuncDeclaration _decl;
};

}
