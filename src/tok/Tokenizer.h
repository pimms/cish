#include "Token.h"
#include "Trie.h"
#include "../Exception.h"

#include <vector>

namespace cish::tok 
{

DECLARE_EXCEPTION(TokenizerError);

class Tokenizer
{
public:
    Tokenizer(const std::string& source);
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer(Tokenizer&&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    Tokenizer& operator=(Tokenizer&&) = delete;

    std::vector<Token> tokenize();

private:
    TokenTrie _trie;
    std::string _source;
    std::vector<Token> _tokens;
    int _pos{};
    int _line{};
    int _col{};

    void reset();

    bool readToken();
    uint32_t readRegexToken(const std::string& strExpr);
    void addToken(TokenType type, uint32_t len);

    void skipToNextNonWS();
    bool skipToNextOccurence(std::string_view needle);
    char peek(int n) const;
    bool match(const std::string_view s);
};

}
