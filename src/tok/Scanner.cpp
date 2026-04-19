#include "Scanner.h"
#include <cassert>
#include <optional>
#include <regex>

namespace cish::tok 
{

Scanner::Scanner(const std::string& source)
    : _source(source)
    , _pos(0)
{
    _trie.insert("(", TokenType::PAREN_L);
    _trie.insert(")", TokenType::PAREN_L);
    _trie.insert("[", TokenType::SQPAREN_L);
    _trie.insert("]", TokenType::SQPAREN_R);
    _trie.insert("{", TokenType::CBRACE_L);
    _trie.insert("}", TokenType::CBRACE_R);
    _trie.insert("<", TokenType::ABRACE_L);
    _trie.insert(">", TokenType::ABRACE_R);
    _trie.insert(";", TokenType::SEMICOLON);
    _trie.insert(":", TokenType::COLON);
    _trie.insert("/", TokenType::RSLASH);
    _trie.insert("!", TokenType::BANG);
    _trie.insert("*", TokenType::STAR);
    _trie.insert("+", TokenType::PLUS);
    _trie.insert("-", TokenType::MINUS);
    _trie.insert("&", TokenType::AMPERSAND);
    _trie.insert("|", TokenType::PIPE);
    _trie.insert("~", TokenType::TILDE);
    _trie.insert("%", TokenType::MODULO);
    _trie.insert("^", TokenType::CARET);
    _trie.insert("=", TokenType::EQUAL);
    _trie.insert("<=", TokenType::CMP_LTEQ);
    _trie.insert(">=", TokenType::CMP_GTEQ);
    _trie.insert("==", TokenType::CMP_EQ);
    _trie.insert("!=", TokenType::CMP_NE);
    _trie.insert("||", TokenType::LOG_OR);
    _trie.insert("&&", TokenType::LOG_AND);
    _trie.insert("++", TokenType::INCREMENT);
    _trie.insert("--", TokenType::DECREMENT);
    _trie.insert("<<", TokenType::LSHIFT);
    _trie.insert(">>", TokenType::RSHIFT);
    _trie.insert("+=", TokenType::PLUS_ASSIGN);
    _trie.insert("-=", TokenType::MINUS_ASSIGN);
    _trie.insert("*=", TokenType::MUL_ASSIGN);
    _trie.insert("/=", TokenType::DIV_ASSIGN);
    _trie.insert("%=", TokenType::MOD_ASSIGN);
    _trie.insert("<<=", TokenType::LS_ASSIGN);
    _trie.insert(">>=", TokenType::RS_ASSIGN);
    _trie.insert("&=", TokenType::BWAND_ASSIGN);
    _trie.insert("|=", TokenType::BWOR_ASSIGN);
    _trie.insert("~=", TokenType::BNEG_ASSIGN);
    _trie.insert("^=", TokenType::BXOR_ASSIGN);
    _trie.insert("//", TokenType::COMMENT_LINE);
    _trie.insert("/*", TokenType::COMMENT_BLOCK);
    _trie.insert("//", TokenType::COMMENT_LINE);
    _trie.insert("/*", TokenType::COMMENT_BLOCK);
    _trie.insert("do", TokenType::DO);
    _trie.insert("while", TokenType::WHILE);
    _trie.insert("for", TokenType::FOR);
    _trie.insert("if", TokenType::IF);
    _trie.insert("else", TokenType::ELSE);
    _trie.insert("return", TokenType::RETURN);
    _trie.insert("break", TokenType::BREAK);
    _trie.insert("continue", TokenType::CONTINUE);
    _trie.insert("typedef", TokenType::TYPEDEF);
    _trie.insert("#include", TokenType::INCLUDE);
    _trie.insert("struct", TokenType::STRUCT);
    _trie.insert("const", TokenType::CONST);
}

std::vector<Token> Scanner::tokenize()
{
    reset();

    while (readToken()) {
        // cool
    }

    return _tokens;
}

void Scanner::reset()
{
    _tokens = {};
    _pos = 0;
    _line = 1;
    _col = 0;
}

bool Scanner::readToken()
{
    skipToNextNonWS();

    if (_pos >= _source.size()) {
        return false;
    }

    const std::span span(_source.c_str() + _pos, _source.size() - _pos);
    const auto result = _trie.search(span);

    if (result.success) {
        if (result.tokenType == TokenType::COMMENT_LINE) {
            return skipToNextOccurence("\n");
        } else if (result.tokenType == TokenType::COMMENT_BLOCK) {
            return skipToNextOccurence("*/");
        }

        Token t {
            result.tokenType.value(),
            std::string_view(_source.c_str() + _pos, result.length),
            _line,
            _col
        };
        _tokens.push_back(t);

        _pos += result.length;
        _col += result.length;
        return true;
    }

    static std::vector<std::tuple<TokenType, std::string>> patterns = {
        { TokenType::IDENTIFIER, "[_a-zA-Z][_a-zA-Z0-9]*" },
        { TokenType::LIT_INT, "[0-9]+" },
        { TokenType::LIT_INT, "0x[a-fA-F0-9]+" },
        { TokenType::LIT_FLOAT, "[0-9]+\\.[0-9]*[fF]?" },
        { TokenType::LIT_FLOAT, "\\.[0-9]+[fF]?" },
        { TokenType::LIT_CHAR, R"('(\\'|[^\r\n]|\\[^\r\n ])')" },
        { TokenType::LIT_STRING, R"("(?:\\[^\r\n ]|[^"\r\n])*")" },
        { TokenType::SYSTEM_MODULE, "<(a-zA-Z0-9/._-)+>" },
    };

    for (const auto& [type, expr]: patterns) {
        if (const uint32_t len = readRegexToken(expr)) {
            addToken(type, len);
            return true;
        }
    }

    return false;
}

uint32_t Scanner::readRegexToken(const std::string& strExpr)
{
    std::regex regex(strExpr);
    std::cmatch match;
    std::regex_search(_source.c_str() + _pos, match, regex, std::regex_constants::match_continuous);

    assert(match.size() < 2 && "The expression should not contain a capture group");

    if (match.size() == 1) {
        return match[0].length();
    }
    return 0;
}

void Scanner::addToken(TokenType type, uint32_t len)
{
    Token token {
        type,
        std::string_view(_source.c_str() + _pos, len),
        _line,
        _col
    };
    _tokens.push_back(token);

    _pos += len;
    _col += len;
}

void Scanner::skipToNextNonWS()
{
    // We handle newlines explicitly to ensure the lineNo-bookkeeping
    // is in order, but rely on stdlib for other whitespace checking.
    while (const char ch = peek(0)) {
        switch (ch) {
            case '\n':
                _line++;
                _pos++;
                _col = 0;
                break;
            default:
                if (std::iswspace(ch)) {
                    _pos++;
                    _col++;
                } else {
                    return;
                }
        }
    }
}

bool Scanner::skipToNextOccurence(std::string_view needle)
{
    const int needleLen = needle.size();
    const int upperLimit =  _source.size() - needleLen;

    int newLine = _line;
    int newCol = _col;

    for (int i=_pos; i<upperLimit; i++) {
        if (_source[i] == '\n') {
            newLine++;
            newCol = 0;
        } else {
            newCol++;
        }

        bool match = true;
        for (int j=0; j<needleLen; j++) {
            if (needle[j] != _source[i+j]) {
                match = false;
                break;
            }
        }
        if (match) {
            _pos = i + needleLen;
            _line = newLine;
            _col = newCol + needleLen - 1;
            return true;
        }
    }

    return false;
}

char Scanner::peek(int n) const
{
    if (_pos + n < _source.size()) {
        return _source[_pos+n];
    } else {
        return 0;
    }
}

bool Scanner::match(const std::string_view s)
{
    const int n = s.size();
    for (int i=0; i<n; i++) {
        if (peek(i) != s[i]) {
            return false;
        }
    }
    return true;
}

}
