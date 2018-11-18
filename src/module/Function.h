#pragma once

#include "../ast/FuncDeclaration.h"
#include "../ast/ExpressionValue.h"
#include "../vm/Callable.h"

#include <memory>


namespace cish
{
namespace module
{


class Function: public vm::Callable
{
public:
    typedef std::shared_ptr<Function> Ptr;

    Function(ast::FuncDeclaration declaration);

    const ast::FuncDeclaration& getDeclaration() const;
    virtual ast::ExpressionValue execute(vm::ExecutionContext *context,
                                         const std::vector<ast::ExpressionValue>& params) const override = 0;
private:
    ast::FuncDeclaration _declaration;
};


}
}