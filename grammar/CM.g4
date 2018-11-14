grammar CM;

root
    : rootBlock EOF
    ;

rootBlock
    : rootItem*
    ;

rootItem
    : functionDefinition
    | functionDeclaration
    | variableDeclaration ';'
    ;


expression
    : expr
    | '(' expr ')'
    ;

expr
    : incdecexpr                                # INCDECEXPR___
    | expr op=( '*' | '/' | '%' ) expr          # MULT_EXPR
    | expr op=( '+' | '-' ) expr                # ADD_EXPR
    | expr op=( '>=' | '<=' | '>' | '<' ) expr  # COMPARE_EXPR
    | expr op=( '==' | '!=' ) expr              # EQUALITY_EXPR
    | expr op=( '&&' | '||' ) expr              # AND_EXPR
    | (String|Integer|Floating|Boolean|Null)    # LITERAL_EXPR
    | Identifier                                # VAR_REF_EXPR
    | functionCall                              # FUNC_CALL_EXPR
    | '&' Identifier                            # ADDROF_EXPR
    ;
incdecexpr
    : Identifier '++'                           # POSTFIX_INC_EXPR
    | '++' Identifier                           # PREFIX_INC_EXPR
    | Identifier '--'                           # POSTFIX_DEC_EXPR
    | '--' Identifier                           # PREFIX_DEC_EXPR
    ;

statement
    : ifStatement
    | assignment ';'
    | functionCallStatement
    | variableDeclaration ';'
    | returnStatement
    | forStatement
    | whileStatement
    | doWhileStatement
    | expressionStatement
    | ';'
    ;

expressionStatement
    : expression ';'
    ;
functionCallStatement
    : functionCall ';'
    ;
returnStatement
    : 'return' expression? ';'
    ;
 ifStatement
    : 'if' '(' expression ')' '{' statement* '}' elseStatement?
    | 'if' '(' expression ')' statement elseStatement?
    ;
elseStatement
    : 'else' '{' statement* '}'
    | 'else' statement
    ;


forStatement
    : 'for' '(' forInitializer? ';' expression? ';' forIterator? ')' '{' statement* '}'
    | 'for' '(' forInitializer? ';' expression? ';' forIterator? ')' statement
    ;
forInitializer
    : variableDeclaration
    | assignment
    ;
forIterator
    : assignment
    | functionCall
    | incdecexpr
    ;

whileStatement
    : 'while' '(' expression ')' statement
    | 'while' '(' expression ')' '{' statement* '}'
    ;
doWhileStatement
    : 'do' '{' statement* '}' 'while' '(' expression ')' ';'
    ;


assignment
        : identifier '=' expression
    ;

variableDeclaration
    : typeIdentifier identifier ('=' expression)?
    ;

functionDeclaration
    : typeIdentifier identifier '(' identifierList ')' ';'
    ;

functionDefinition
    : typeIdentifier identifier '(' identifierList ')' '{' statement* '}'
    ;

functionCall
    : identifier '(' expressionList ')'
    ;

expressionList
    : ( expression ( ',' expression )* )?
    ;

identifierList
    : ( functionParameter ( ',' functionParameter )* )?
    ;

functionParameter
    : typeIdentifier identifier?
    ;

identifier
    : Identifier
    ;

typeIdentifier
    : mutableTypeIdentifier
    | constTypeIdentifier
    ;
mutableTypeIdentifier
    : identifier Asterisk*
    ;
constTypeIdentifier
    : Const identifier Asterisk*
    ;



Asterisk : '*';

Or       : '||';
And      : '&&';
Equals   : '==';
NEquals  : '!=';
GTEquals : '>=';
LTEquals : '<=';
GT       : '>';
LT       : '<';
Add      : '+';
Subtract : '-';
Multiply : Asterisk;
Divide   : '/';
Modulus  : '%';
OBrace   : '{';
CBrace   : '}';
SColon   : ';';
Assign   : '=';
Null     : 'NULL';
Const    : 'const';

Integer
    : [0-9]+
    ;

Floating
    : [0-9]+.[0-9]+[f]?
    ;

Boolean
    : 'true'
    | 'false'
    ;

String
    : ["] ( ~["\r\n\\] | '\\' ~[\r\n] )* ["]
    | ['] ( ~['\r\n\\] | '\\' ~[\r\n] )* [']
    ;


Identifier
    : [_a-zA-Z][_a-zA-Z0-9]*
    ;



Comment
    : ( '//' ~[\r\n]* | '/*' .*? '*/' ) -> skip
    ;

Space
    : [ \t\r\n\u000C] -> skip
    ;
