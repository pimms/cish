#pragma once

#include "Ast.h"


namespace cish
{
namespace ast
{


DECLARE_EXCEPTION(FunctionNotDefinedException);


class AntlrContext;

class AstBuilder
{
public:
    AstBuilder(const AntlrContext *antlrContext);

    Ast::Ptr buildAst();

private:
    const AntlrContext *_antlrContext;
};


}
}
