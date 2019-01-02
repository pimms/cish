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

protected:
	void virtualExecute(vm::ExecutionContext *context) const override;
};


}
}
