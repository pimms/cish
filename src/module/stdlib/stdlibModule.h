#pragma once

#include "MallocContext.h"

#include "../Module.h"
#include "../Function.h"
#include "../../ast/FuncDeclaration.h"
#include "../../Exception.h"

namespace cish
{
namespace module
{
namespace stdlib
{

DECLARE_EXCEPTION(StdlibException);


Module::Ptr buildModule();


namespace impl
{



class Atof: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Atof();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};

class Atoi: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Atoi();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};

class Rand: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Rand();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};

class Srand: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Srand();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
};

class Malloc: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Malloc(MallocContext::Ptr mallocContext);
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
private:
    MallocContext::Ptr _mallocContext;
};

class Free: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Free(MallocContext::Ptr mallocContext);
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params) const override;
private:
    MallocContext::Ptr _mallocContext;
};


}
}
}
}