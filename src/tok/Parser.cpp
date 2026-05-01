#include "Parser.h"

namespace cish::tok
{

Parser::Parser(std::vector<Token>& tokens)
    :   _context { tokens }
{ }

void Parser::parse()
{
    reset();
}

void Parser::reset()
{
    _context.reset();
}

void Parser::visitSystemInclude(const Token* token)
{
    auto it = token->getLexeme().begin();
    while (*it++ != '<') { }
    auto last = token->getLexeme().end() - 1;
    std::string moduleName(it, last);
    includeModule(moduleName);
}

}
