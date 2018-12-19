#include "AstNodes.h"
#include "../Exception.h"
#include "../vm/ExecutionContext.h"


namespace cish
{
namespace ast
{


void AstNode::setSourcePosition(const SourcePosition &pos)
{
    _sourcePos = pos;
}

SourcePosition AstNode::getSourcePosition() const
{
    return _sourcePos;
}



void Statement::execute(vm::ExecutionContext *context) const
{
    context->onStatementEnter(this);
}



void NoOpStatement::execute(vm::ExecutionContext *context) const
{
	Statement::execute(context);
}


}
}
