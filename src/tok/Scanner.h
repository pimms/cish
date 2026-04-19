#include "Token.h"
#include "Trie.h"

#include <vector>

namespace cish::tok 
{

class Scanner
{
public:
    Scanner(const std::string& source);
    Scanner(const Scanner&) = delete;
    Scanner(Scanner&&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    Scanner& operator=(Scanner&&) = delete;

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
