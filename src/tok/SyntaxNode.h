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

private:
    std::vector<Token>& _tokens;
};

}
