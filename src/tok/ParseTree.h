#pragma once

#include <string>
#include <vector>

namespace cish::tok
{

struct ForLoopInitializer {};
struct RootItem {};
struct Statement {};
struct Expression: public ForLoopInitializer {};

struct Identifier {
    std::string name;
};
struct TypeIdentifier {
    bool isConst {};
    bool isStruct {};
    std::string type;
    // The number of *'s in the type. E.g., for "char**", pointerLevel is 2.
    int pointerLevel {};
};
struct FunctionParameter {
    TypeIdentifier type;
    std::optional<Identifier> name;
};
struct StructFieldDeclaration {
    TypeIdentifier type;
    Identifier name;
};

/*
================
EXPRESSIONS
================
*/
enum class UnaryOperator {
    POSTINC,    POSTDEC,
    PREINC,     PREDEC,
    MINUS,
    NEGATE,
    ONES_COMPL,
    DEREF,
    ADDROF,
    SIZEOF,
};
enum class BinaryOperator {
    MULT,       DIVIDE,     MODULO,
    PLUS,       MINUS,
    LSHIFT,     RSHIFT,
    GTE,        LTE,        GT,         LT,
    EQUALS,     NEQUALS,
    BITAND,     BITXOR,     BITOR,
    LOGAND,     LOGOR,
};
enum class ArithmeticAssignmentOperator {
    MULT,       DIVIDE,     MODULO,
    PLUS,       MINUS,
    LSHIFT,     RSHIFT,
    BITAND,     BITXOR,     BITOR,
};
struct SubscriptExpr: public Expression {
    Expression ptrExpr;
    Expression idxExpr;
};
struct StringliteralExpr: public Expression {
    std::string value;
};
struct FunctionCallExpr: public Expression {
    std::string functionName;
    std::vector<Expression> params;
};
struct VarRefExpr: public Expression {
    Identifier identifier;
};
struct BinaryExpr: public Expression {
    Expression left;
    Expression right;
    BinaryOperator oper;
};
struct BoolLiteralExpr: public Expression {
    bool value;
};
struct CharLiteralExpr: public Expression {
    char value;
};
struct IntLiteralExpr: public Expression {
    int64_t value;
};
struct FloatLiteralExpr: public Expression {
    double value;
};
struct StringLiteralExpr: public Expression {
    std::string value;
};


/*
================
STATEMENTS
================
*/
struct IfStatement: public Statement {
    Expression condition;
    std::vector<Statement> body;
};
struct AssignmentStatement: public Statement, public ForLoopInitializer {
    Expression left;
    Expression right;
};
struct VariableDeclarationStatement: public Statement, public ForLoopInitializer, public RootItem {
    TypeIdentifier type;
    Identifier varName;
    std::optional<Expression> expression;
};
struct ArithmeticAssignmentStatement: public Statement, public ForLoopInitializer {
    Expression left;
    ArithmeticAssignmentOperator oper;
    Expression right;
};
struct ReturnStatement: public Statement {
    std::optional<Expression> expression;
};
struct ForStatement: public Statement {
    std::optional<ForLoopInitializer> initializer;
    std::optional<Expression> condition;
    std::optional<Expression> update;
};
struct WhileStatement: public Statement {
    Expression condition;
    std::vector<Statement> body;
};
struct DoWhileStatement: public Statement {
    Expression condition;
    std::vector<Statement> body;
};
struct ExpressionStatement: public Statement {
    Expression expression;
};

/*
================
ROOT ITEMS
================
*/
struct FunctionDefinition: public RootItem {
    TypeIdentifier returnType;
    Identifier name;
    std::vector<FunctionParameter> params;
    std::vector<Statement> body;
};
struct FunctionDeclaration: public RootItem {
    TypeIdentifier returnType;
    Identifier name;
    std::vector<FunctionParameter> params;
};
struct SystemInclude: public RootItem {
    std::string moduleName;
};
struct StructDeclaration: public RootItem {
    Identifier name;
    std::vector<StructFieldDeclaration> fields;
};

struct Root {
    std::vector<RootItem> rootItems;
};



}
