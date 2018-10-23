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
    | variableDeclaration
    ;


expression
    : expression op=( '*' | '/' | '%' ) expression
    | expression op=( '+' | '-' ) expression
    | expression op=( '>=' | '<=' | '>' | '<' ) expression
    | expression op=( '==' | '!=' ) expression
    | expression '&&' expression
    | expression '||' expression
    | String
    | Integer
    | Floating
    | Boolean
    | Null
    | Identifier
    | functionCall
    | '(' expression op=( '+' | '-' ) expression ')'
    | '(' expression op=( '>=' | '<=' | '>' | '<' ) expression ')'
    | '(' expression op=( '==' | '!=' ) expression ')'
    | '(' expression '&&' expression ')'
    | '(' expression '||' expression ')'
    | '(' String ')'
    | '(' Integer ')'
    | '(' Floating ')'
    | '(' Boolean ')'
    ;

statement
    : ifStatement
    | assignment
    | functionCall ';'
    | variableDeclaration
    | returnStatement
    | ';'
    ;

returnStatement
    : 'return' expression ';'
    ;

ifStatement
    : 'if' '(' expression ')' '{' statement* '}'
    | 'if' '(' expression ')' statement
    ;
elseStatement
    : 'else' '{' statement* '}'
    | 'else' statement
    ;

assignment
    : (typeIdentifier)? Identifier '=' expression ';'
    ;

variableDeclaration
    : typeIdentifier Identifier ('=' expression)? ';'
    ;

functionDeclaration
    : typeIdentifier Identifier '(' identifierList ')' ';'
    ;

functionDefinition
    : typeIdentifier Identifier '(' identifierList ')' '{' statement* '}'
    ;

functionCall
    : Identifier '(' expressionList ')'
    ;

expressionList
    : ( expression ( ',' expression )* )?
    ;

identifierList
    : ( typeIdentifier Identifier? ( ',' typeIdentifier Identifier? )* )?
    ;




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
Multiply : '*';
Divide   : '/';
Modulus  : '%';
OBrace   : '{';
CBrace   : '}';
SColon   : ';';
Assign   : '=';
Null     : 'NULL';

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

typeIdentifier
    : ('const')? Identifier ('*')?
    | 'void'
    ;



Comment
    : ( '//' ~[\r\n]* | '/*' .*? '*/' ) -> skip
    ;

Space
    : [ \t\r\n\u000C] -> skip
    ;
