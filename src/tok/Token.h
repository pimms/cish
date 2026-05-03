#pragma once

#include "TokenType.h"
#include <ostream>
#include <string>

namespace cish::tok
{

struct Token 
{
public:
    Token(TokenType type, std::string_view lexeme, int line, int col);

    bool operator==(const Token& o) const = default;

    TokenType getType() const;
    const std::string& getLexeme() const;
    int getLine() const;
    int getCol() const;

    std::string toString() const;

private:
    TokenType _type;
    std::string _lexeme;
    int _line;
    int _col;
};

std::ostream& operator<<(std::ostream&, const Token&);

}
