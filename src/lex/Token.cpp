#include "Token.h"
#include <sstream>

namespace cish::lex
{

Token::Token(TokenType type, std::string_view lexeme, int line, int col, int charOffset)
    : _type(type)
    , _lexeme(lexeme)
    , _line(line)
    , _col(col)
    , _charOffset(charOffset)
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

int Token::getCharOffset() const
{
    return _charOffset;
}

std::string Token::toString() const
{
    std::ostringstream ss;
    ss << *this;
    std::string s = ss.str();
    return s;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    return os
        << "Token[type=" << token.getType()
        << ",lexeme=\"" << token.getLexeme()
        << "\",line=" << token.getLine()
        << ",col=" << token.getCol()
        << "]";
}

}
