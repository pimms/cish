#include "Token.h"

namespace cish::tok 
{

Token::Token(TokenType type, std::string_view lexeme, int line, int col)
    : _type(type)
    , _lexeme(lexeme)
    , _line(line)
    , _col(col)
{

}

TokenType Token::getType() const
{
    return _type;
}

const std::string& Token::getLexeme() const
{
    return _lexeme;
}

int Token::getLine() const
{
    return _line;
}

int Token::getCol() const 
{
    return _col;
}

}
