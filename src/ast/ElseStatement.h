#pragma once

#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class ElseStatement: public SuperStatement
{
public:
    typedef std::shared_ptr<ElseStatement> Ptr;

	void execute(vm::ExecutionContext *context) const override;
};


}
}
