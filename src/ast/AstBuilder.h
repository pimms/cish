#pragma once

#include "Ast.h"
#include "ParseContext.h"

#include "../module/ModuleContext.h"


namespace cish::ast
{

class AntlrContext;

class AstBuilder
{
public:
    AstBuilder(const ParseContext::Ptr antlrContext,
               const module::ModuleContext::Ptr moduleContext);

    Ast::Ptr buildAst();

private:
    const ParseContext::Ptr _parseContext;
    module::ModuleContext::Ptr _moduleContext;
};

}
