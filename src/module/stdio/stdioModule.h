#pragma once

#include "../Module.h"
#include "../Function.h"
#include "../../ast/FuncDeclaration.h"
#include "../../Exception.h"

namespace cish
{
namespace module
{
namespace stdio
{


DECLARE_EXCEPTION(StdioException);

Module::Ptr buildModule();


namespace impl
{


class Puts: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Puts();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};

class Printf: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Printf();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};



}
}
}
}