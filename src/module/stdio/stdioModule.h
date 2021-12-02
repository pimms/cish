#pragma once

#include "FopenContext.h"

#include "../Module.h"
#include "../Function.h"
#include "../../ast/FuncDeclaration.h"
#include "../../ast/StructLayout.h"
#include "../../Exception.h"

namespace cish
{
namespace ast
{
class StructLayout;
}

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
                                 FuncParams params,
                                 vm::Variable*) const override;
};

class Printf: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Printf();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};

class Fopen: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Fopen(FopenContext::Ptr fopenContext);

    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;

private:
    FopenContext::Ptr _fopenContext;
};

class Fclose: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Fclose(FopenContext::Ptr fopenContext);

    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;

private:
    FopenContext::Ptr _fopenContext;
};

class Fgetc: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Fgetc(FopenContext::Ptr fopenContext);

    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;

private:
    FopenContext::Ptr _fopenContext;
};

class Fgets: public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Fgets(FopenContext::Ptr fopenContext);

    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;

private:
    FopenContext::Ptr _fopenContext;
};


}
}
}
}
