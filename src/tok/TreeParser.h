#pragma once

#include "Token.h"

namespace cish::tok
{

class TreeParser
{
public:
    TreeParser(std::vector<Token>& tokens);
    TreeParser() = delete;
    TreeParser(const TreeParser&) = delete;
    TreeParser& operator=(const TreeParser&) = delete;
    TreeParser& operator=(TreeParser&&) = delete;
    ~TreeParser() = default;

private:
    std::vector<Token>& _tokens;


};

}
