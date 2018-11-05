#include "AstNodes.h"
#include "../Exception.h"


namespace cish
{
namespace ast
{


/*
==============
Statement
==============
*/
Statement::Statement(SuperStatement *super):
    _superStatement(super)
{ }

SuperStatement* Statement::getSuperStatement() const
{
    return _superStatement;
}


}
}
