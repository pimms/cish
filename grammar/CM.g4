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
    : expr
    | '(' expr ')'
    ;

expr
    : expr op=( '*' | '/' | '%' ) expr          # MULT_EXPR
    | expr op=( '+' | '-' ) expr                # ADD_EXPR
    | expr op=( '>=' | '<=' | '>' | '<' ) expr  # COMPARE_EXPR
    | expr op=( '==' | '!=' ) expr              # EQUALITY_EXPR
    | expr op=( '&&' | '||' ) expr              # AND_EXPR
    | (String|Integer|Floating|Boolean|Null)    #LITERAL_EXPR
    | Identifier                                # VAR_REF_EXPR
    | functionCall                              # FUNC_CALL_EXPR
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
    : (typeIdentifier)? identifier '=' expression ';'
    ;

variableDeclaration
    : typeIdentifier identifier ('=' expression)? ';'
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
    : ( typeIdentifier identifier? ( ',' typeIdentifier identifier? )* )?
    ;


identifier
    : Identifier
    ;

typeIdentifier
    : mutableTypeIdentifier
    | constTypeIdentifier
    | 'void'
    ;
mutableTypeIdentifier
    : identifier ('*')?
    ;
constTypeIdentifier
    : 'const' identifier ('*')?
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



Comment
    : ( '//' ~[\r\n]* | '/*' .*? '*/' ) -> skip
    ;

Space
    : [ \t\r\n\u000C] -> skip
    ;
