#pragma once

namespace cish::tok
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
    DQUOTE,             // "
    SQUOTE,             // '
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

    COMMENT_LINE_BEG,   // //
    COMMENT_BLOCK_BEG,  // /*

    COMMENT_LINE,       // // blablabla
    COMMENT_BLOCK,      // /* blablabla */

    DO,
    WHILE,
    FOR,
    IF,
    ELSE,
    RETURN,
    BREAK,
    CONTINUE,
    TYPEDEF,
    INCLUDE,
    STRUCT,

    LIT_NULL,
    LIT_STRING,
    LIT_INT,
    LIT_CHAR,
    LIT_FLOAT,
    
    IDENTIFIER, 
};

}
