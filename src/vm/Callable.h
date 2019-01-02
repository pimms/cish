#pragma once

#include "../ast/ExpressionValue.h"
#include "../ast/FuncDeclaration.h"
#include <vector>
#include <memory>

namespace cish
{
namespace vm
{

class ExecutionContext;

class Callable
{
public:
    typedef std::shared_ptr<Callable> Ptr;

    virtual ~Callable() = default;
    virtual const ast::FuncDeclaration* getDeclaration() const = 0;
    virtual ast::ExpressionValue execute(ExecutionContext *context,
                                         const std::vector<ast::ExpressionValue>& params,
                                         vm::Variable *returnBuffer) const = 0;
};


}
}
