#pragma once

#include "TokenType.h"
#include <ostream>
#include <string>

namespace cish::tok
{

class Token
{
public:
    explicit Token(TokenType type, std::string_view lexeme, int line, int col, int charOffset);

    bool operator==(const Token& o) const = default;

    TokenType getType() const;
    const std::string& getLexeme() const;
    int getLine() const;
    int getCol() const;
    int getCharOffset() const;

    std::string toString() const;

private:
    TokenType _type;
    std::string _lexeme;
    int _line;
    int _col;
    int _charOffset;
};

std::ostream& operator<<(std::ostream&, const Token&);

}
