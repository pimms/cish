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
    | systemInclude
    | structDeclaration
    ;


expression
    : expr
    ;

expr
    : '(' expr ')'                              # PAREN_EXPR___ // Not to be used explicitly
    | expr op=( '.' | '->' ) identifier         # STRUCT_ACCESS_EXPR
    | expr '[' expr ']'                         # SUBSCRIPT_EXPR
    | expr '++'                                 # POSTFIX_INC_EXPR
    | expr '--'                                 # POSTFIX_DEC_EXPR
    | '++' expr                                 # PREFIX_INC_EXPR
    | '--' expr                                 # PREFIX_DEC_EXPR
    | '-' expr                                  # MINUS_EXPR
    | '!' expr                                  # NEGATION_EXPR
    | '~' expr                                  # ONES_COMPLEMENT_EXPR
    | '(' typeIdentifier ')' expr               # TYPE_CAST_EXPR
    | '*' expr                                  # DEREF_EXPR
    | '&' expr                                  # ADDROF_EXPR
    | 'sizeof' sizeofTerm                       # SIZEOF_EXPR
    | expr op=( '*' | '/' | '%' ) expr          # MULT_EXPR
    | expr op=( '+' | '-' ) expr                # ADD_EXPR
    | expr op=( '<<' | '>>' ) expr              # BITSHIFT_EXPR
    | expr op=( '>=' | '<=' | '>' | '<' ) expr  # COMPARE_EXPR
    | expr op=( '==' | '!=' ) expr              # EQUALITY_EXPR
    | expr op=( '&' | '^' | '|' ) expr          # BITWISE_EXPR
    | expr op=( '&&' | '||' ) expr              # AND_EXPR
    | exprAtom                                  # EXPR_ATOM___ // Not to be used explicitly
    ;
exprAtom
    : (Char|Integer|Floating|Boolean|Null)      # LITERAL_EXPR
    | Identifier                                # VAR_REF_EXPR
    | functionCall                              # FUNC_CALL_EXPR
    | stringLiteral                             # STR_LITERAL_EXPR
    ;

statement
    : ifStatement
    | assignment ';'
    | variableDeclaration ';'
    | arithmeticAssignment ';'
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
    : arithmeticAssignment
    | assignment
    | expression
    ;

whileStatement
    : 'while' '(' expression ')' statement
    | 'while' '(' expression ')' '{' statement* '}'
    ;
doWhileStatement
    : 'do' '{' statement* '}' 'while' '(' expression ')' ';'
    ;


assignment
    : expression '=' expression
    ;

arithmeticAssignment
    : expression  op=( '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '|=' ) expression
    ;

variableDeclaration
    : typeIdentifier identifier ('=' expression)?
    ;

structDeclaration
    : Struct identifier '{' structFieldDeclaration* '}' ';'
    ;
structFieldDeclaration
    : typeIdentifier identifier ';'
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
    : (Const)? typeName Asterisk*
    ;
typeName
    : 'bool'
    | 'char'
    | 'int'
    | 'long'
    | 'short'
    | 'float'
    | 'double'
    | 'void'
    | Struct identifier
    ;

sizeofTerm
    : '(' sizeofTerm ')'
    | typeIdentifier
    | exprAtom
    | expr
    ;

stringLiteral
    : String
    ;

systemInclude
    : '#include' sysModuleName
    ;
sysModuleName
    : SysModuleName
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
Struct   : 'struct';

Integer
    : [0-9]+
    | '0x'[a-fA-F0-9]+
    ;
Floating
    : [0-9]+ '.' [0-9]* ('f' | 'F')?
    | '.' [0-9]+ ('f' | 'F')?
    ;
Boolean
    : 'true'
    | 'false'
    ;
String
    : ["] ( ~["\r\n\\] | '\\' ~[\r\n] )* ["]
    ;
Char
    : ['] ( ~['\r\n\\] | '\\' ~[\r\n] )* [']
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

SysModuleName
    : '<' ('a'..'z'|'A'..'Z'|'0'..'9'|'/'|'.'|'_'|'-')+ '>'
    ;
