#pragma once

#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class ElseStatement: public SuperStatement
{
public:
	void execute(vm::ExecutionContext *context) const override;
};


}
}