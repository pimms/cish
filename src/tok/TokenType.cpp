#include "TokenType.h"
#include <cassert>

namespace cish::tok 
{

std::ostream& operator<<(std::ostream& s, const TokenType& tokenType)
{
    switch (tokenType) {
        case TokenType::PAREN_L: return s << "PAREN_L";
        case TokenType::PAREN_R: return s << "PAREN_R";
        case TokenType::SQPAREN_L: return s << "SQPAREN_L";
        case TokenType::SQPAREN_R: return s << "SQPAREN_R";
        case TokenType::CBRACE_L: return s << "CBRACE_L";
        case TokenType::CBRACE_R: return s << "CBRACE_R";
        case TokenType::ABRACE_L: return s << "ABRACE_L";
        case TokenType::ABRACE_R: return s << "ABRACE_R";
        case TokenType::SEMICOLON: return s << "SEMICOLON";
        case TokenType::COLON: return s << "COLON";
        case TokenType::RSLASH: return s << "RSLASH";
        case TokenType::BANG: return s << "BANG";
        case TokenType::STAR: return s << "STAR";
        case TokenType::PLUS: return s << "PLUS";
        case TokenType::MINUS: return s << "MINUS";
        case TokenType::AMPERSAND: return s << "AMPERSAND";
        case TokenType::PIPE: return s << "PIPE";
        case TokenType::TILDE: return s << "TILDE";
        case TokenType::MODULO: return s << "MODULO";
        case TokenType::CARET: return s << "CARET";
        case TokenType::EQUAL: return s << "EQUAL";
        case TokenType::COMMA: return s << "COMMA";
        case TokenType::DOT: return s << "DOT";
        case TokenType::ARROW: return s << "ARROW";
        case TokenType::CMP_LTEQ: return s << "CMP_LTEQ";
        case TokenType::CMP_GTEQ: return s << "CMP_GTEQ";
        case TokenType::CMP_EQ: return s << "CMP_EQ";
        case TokenType::CMP_NE: return s << "CMP_NE";
        case TokenType::LOG_OR: return s << "LOG_OR";
        case TokenType::LOG_AND: return s << "LOG_AND";
        case TokenType::INCREMENT: return s << "INCREMENT";
        case TokenType::DECREMENT: return s << "DECREMENT";
        case TokenType::LSHIFT: return s << "LSHIFT";
        case TokenType::RSHIFT: return s << "RSHIFT";
        case TokenType::PLUS_ASSIGN: return s << "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return s << "MINUS_ASSIGN";
        case TokenType::MUL_ASSIGN: return s << "MUL_ASSIGN";
        case TokenType::DIV_ASSIGN: return s << "DIV_ASSIGN";
        case TokenType::MOD_ASSIGN: return s << "MOD_ASSIGN";
        case TokenType::LS_ASSIGN: return s << "LS_ASSIGN";
        case TokenType::RS_ASSIGN: return s << "RS_ASSIGN";
        case TokenType::BWAND_ASSIGN: return s << "BWAND_ASSIGN";
        case TokenType::BWOR_ASSIGN: return s << "BWOR_ASSIGN";
        case TokenType::BNEG_ASSIGN: return s << "BNEG_ASSIGN";
        case TokenType::BXOR_ASSIGN: return s << "BXOR_ASSIGN";
        case TokenType::COMMENT_LINE: return s << "COMMENT_LINE";
        case TokenType::COMMENT_BLOCK: return s << "COMMENT_BLOCK";
        case TokenType::INCLUDE_SYS: return s << "INCLUDE_SYS";
        case TokenType::DO: return s << "DO";
        case TokenType::WHILE: return s << "WHILE";
        case TokenType::FOR: return s << "FOR";
        case TokenType::IF: return s << "IF";
        case TokenType::ELSE: return s << "ELSE";
        case TokenType::RETURN: return s << "RETURN";
        case TokenType::BREAK: return s << "BREAK";
        case TokenType::CONTINUE: return s << "CONTINUE";
        case TokenType::TYPEDEF: return s << "TYPEDEF";
        case TokenType::STRUCT: return s << "STRUCT";
        case TokenType::CONST: return s << "CONST";
        case TokenType::LIT_NULL: return s << "LIT_NULL";
        case TokenType::LIT_STRING: return s << "LIT_STRING";
        case TokenType::LIT_INT: return s << "LIT_INT";
        case TokenType::LIT_CHAR: return s << "LIT_CHAR";
        case TokenType::LIT_FLOAT: return s << "LIT_FLOAT";
        case TokenType::IDENTIFIER: return s << "IDENTIFIER";
        default:
            assert(0);
            return s << "UNKNOWN";
    }
}

}
