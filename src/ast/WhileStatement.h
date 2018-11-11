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
	WhileStatement(Expression *condition);
	virtual ~WhileStatement();

	virtual void execute(vm::ExecutionContext *context) const override;

protected:
	bool evaluateCondition(vm::ExecutionContext *context) const;

private:
	Expression *_condition;
};


class DoWhileStatement: public WhileStatement
{
public:
	DoWhileStatement(Expression *condition);
	virtual void execute(vm::ExecutionContext *context) const override;
};


}
}