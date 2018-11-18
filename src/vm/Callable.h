#pragma once

#include "../ast/ExpressionValue.h"
#include <vector>

namespace cish
{
namespace vm
{

class ExecutionContext;

class Callable
{
public:
    virtual ast::ExpressionValue execute(ExecutionContext *context,
                                         const std::vector<ast::ExpressionValue>& params) const = 0;
};


}
}