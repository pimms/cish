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
    Trie _trie;
    std::string _source;
    std::vector<Token> _tokens;
    int _pos{};
    int _line{};
    int _col{};

    void reset();
    bool readToken();
    void addToken(TokenType t, int lexemeLen);

    void skipToNextNonWS();

    char peek() const;
    char peek(int n) const;
    bool match(const std::string_view s);
};

}
