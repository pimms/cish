#pragma once

#include "../Module.h"

namespace cish
{
namespace module
{
namespace string
{


Module::Ptr buildModule();

namespace impl
{


/*
==================
void *memchr(const void *str, int c, int n)
==================
*/
class Memchr : public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Memchr();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};



}

}
}
}
