#pragma once

#include "AstNodes.h"
#include "SuperStatement.h"


namespace cish::ast
{

class WhileStatement: public SuperStatement
{
public:
	WhileStatement(Expression::Ptr condition);
	virtual ~WhileStatement() = default;

protected:
	virtual void virtualExecute(vm::ExecutionContext *context) const override;
	bool evaluateCondition(vm::ExecutionContext *context) const;

private:
	Expression::Ptr _condition;
};


class DoWhileStatement: public WhileStatement
{
public:
	DoWhileStatement(Expression::Ptr condition);

protected:
	virtual void virtualExecute(vm::ExecutionContext *context) const override;
};

}
