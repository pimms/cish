#pragma once

#include "TokenContext.h"
#include "AST.h"

namespace cish::tok
{

class Parser
{
public:
    Parser(std::vector<Token>& tokens);
    Parser(const Parser& o) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;
    Parser& operator=(const Parser&) = delete;

    void parse();

private:
    TokenContext _context;

    void reset();

    void visitSystemInclude(const Token* token);

    void includeModule(const std::string& name);
};

}
