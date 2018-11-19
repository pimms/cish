#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"


namespace cish
{
namespace ast
{


class WhileStatement: public SuperStatement
{
public:
	WhileStatement(Expression::Ptr condition);
	virtual ~WhileStatement() = default;

	virtual void execute(vm::ExecutionContext *context) const override;

protected:
	bool evaluateCondition(vm::ExecutionContext *context) const;

private:
	Expression::Ptr _condition;
};


class DoWhileStatement: public WhileStatement
{
public:
	DoWhileStatement(Expression::Ptr condition);
	virtual void execute(vm::ExecutionContext *context) const override;
};


}
}