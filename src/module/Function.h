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
    Function(ast::FuncDeclaration declaration);
    virtual ~Function() = default;

    const ast::FuncDeclaration* getDeclaration() const override;
    virtual ast::ExpressionValue execute(vm::ExecutionContext *context,
                                         const std::vector<ast::ExpressionValue>& params) const override = 0;
private:
    ast::FuncDeclaration _declaration;
};


}
}