#include "Scanner.h"
#include <cassert>

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
    _trie.insert("\"", TokenType::DQUOTE);
    _trie.insert("'", TokenType::SQUOTE);
    _trie.insert("!", TokenType::BANG);
    _trie.insert("*", TokenType::STAR);
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
    _trie.insert("//", TokenType::COMMENT_LINE_BEG);
    _trie.insert("/*", TokenType::COMMENT_BLOCK_BEG);
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
}

std::vector<Token> Scanner::tokenize()
{
    reset();

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
    const char ch = peek(0);
    if (!ch) return false;

    return true;
}

void Scanner::addToken(TokenType type, int lexemeLen)
{
    const std::string_view lexeme(_source.c_str() + _pos, lexemeLen);
    Token token(type, lexeme, _line, _col);
    _tokens.push_back(token);
}

void Scanner::skipToNextNonWS()
{
    // We handle newlines explicitly to ensure the lineNo-bookkeeping
    // is in order, but rely on stdlib for other whitespace checking.
    while (const char ch = peek(1)) {
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
