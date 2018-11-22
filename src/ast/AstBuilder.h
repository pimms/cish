#pragma once

#include "Ast.h"
#include "../module/ModuleContext.h"


namespace cish
{
namespace ast
{


DECLARE_EXCEPTION(FunctionNotDefinedException);
DECLARE_EXCEPTION(ModuleNotFoundException);


class AntlrContext;

class AstBuilder
{
public:
    AstBuilder(const AntlrContext *antlrContext,
               const module::ModuleContext::Ptr moduleContext);

    Ast::Ptr buildAst();

private:
    const AntlrContext *_antlrContext;
    module::ModuleContext::Ptr _moduleContext;
};


}
}
