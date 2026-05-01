#pragma once

#include <vector>
#include "Token.h"

namespace cish::tok
{

class TokenContext
{
public:
    TokenContext(std::vector<Token>& tokens);
    TokenContext() = delete;
    TokenContext(const TokenContext&) = delete;
    TokenContext& operator=(const TokenContext&) = delete;
    TokenContext& operator=(TokenContext&&) = delete;
    ~TokenContext() = default;

    void reset();

    void begin();
    void revert();
    void commit();

    const Token* peek() const;
    const Token* take();

private:
    std::vector<Token> _tokens;
    int _backIndex;
    int _index;
};

}
