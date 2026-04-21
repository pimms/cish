#pragma once

#include "Ast.h"

#include "../module/ModuleContext.h"
#include "AntlrContext.h"

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
