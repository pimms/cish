#pragma once

#include "../ast/Ast.h"

#include "../AntlrContext.h"
#include "../module/ModuleContext.h"

namespace cish::ast
{

class AntlrContext;

class AstBuilder
{
public:
    AstBuilder(const AntlrContext::Ptr antlrContext,
               const module::ModuleContext::Ptr moduleContext);

    Ast::Ptr buildAst();

private:
    const AntlrContext::Ptr _parseContext;
    module::ModuleContext::Ptr _moduleContext;
};

}
