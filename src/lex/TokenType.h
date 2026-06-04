#pragma once

#include <ostream>

namespace cish::lex
{

enum class TokenType 
{
    PAREN_L,            // (
    PAREN_R,            // )
    SQPAREN_L,          // [
    SQPAREN_R,          // ]
    CBRACE_L,           // {
    CBRACE_R,           // }
    ABRACE_L,           // <
    ABRACE_R,           // >
    SEMICOLON,          // ;
    COLON,              // :
    RSLASH,             // /
    BANG,               // !
    STAR,               // *
    PLUS,               // +
    MINUS,              // -
    AMPERSAND,          // &
    PIPE,               // |
    TILDE,              // ~
    MODULO,             // %
    CARET,              // ^
    EQUAL,              // =

    COMMA,              // ,
    DOT,                // .
    ARROW,              // ->

    CMP_LTEQ,           // <=
    CMP_GTEQ,           // >=
    CMP_EQ,             // ==
    CMP_NE,             // !=

    LOG_OR,             // ||
    LOG_AND,            // &&

    INCREMENT,          // ++
    DECREMENT,          // --
    LSHIFT,             // <<
    RSHIFT,             // >>
    PLUS_ASSIGN,        // +=
    MINUS_ASSIGN,       // -=
    MUL_ASSIGN,         // *=
    DIV_ASSIGN,         // /=
    MOD_ASSIGN,         // %=
    LS_ASSIGN,          // <<=
    RS_ASSIGN,          // >>=
    BWAND_ASSIGN,       // &=
    BWOR_ASSIGN,        // |=
    BNEG_ASSIGN,        // ~=
    BXOR_ASSIGN,        // ^=

    COMMENT_LINE,       // //
    COMMENT_BLOCK,      // /*

    INCLUDE_SYS,        // #include <inet/inet.h>

    DO,
    WHILE,
    FOR,
    IF,
    ELSE,
    RETURN,
    BREAK,
    CONTINUE,
    TYPEDEF,
    STRUCT,
    CONST,
    SIZEOF,

    LIT_NULL,
    LIT_STRING,
    LIT_INT,
    LIT_CHAR,
    LIT_FLOAT,

    VOID,
    BOOL,
    CHAR,
    SHORT,
    INT,
    LONG,
    UNSIGNED,
    FLOAT,
    DOUBLE,
    TYPE_NAME,

    IDENTIFIER,
    END_OF_FILE,
};

std::ostream& operator<<(std::ostream&, const TokenType&);

}
