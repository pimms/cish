#pragma once

#include "Token.h"
#include "Trie.h"
#include "../Exception.h"

#include <vector>

namespace cish::lex
{

DECLARE_EXCEPTION(TokenizerError);

class Lexer
{
public:
    explicit Lexer(const std::string& source);
    Lexer() = delete;
    Lexer(const Lexer&) = delete;
    Lexer(Lexer&&) = delete;
    Lexer& operator=(const Lexer&) = delete;
    Lexer& operator=(Lexer&&) = delete;

    std::vector<Token> tokenize();

private:
    TokenTrie _trie;
    std::string _source;
    std::vector<Token> _tokens;
    uint32_t _pos{};
    uint32_t _line{};
    uint32_t _col{};

    void reset();

    bool readToken();
    std::optional<std::tuple<TokenType,uint32_t>> readRegexToken() const;
    std::optional<TokenType> keywordFromIdentifier(std::string_view identifier) const;
    void addToken(TokenType type, uint32_t len);

    void skipToNextNonWS();
    bool skipToNextOccurrence(std::string_view needle);
    char peek(int n) const;
    bool match(std::string_view s);
};

}
