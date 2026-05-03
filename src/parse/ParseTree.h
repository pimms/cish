#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace cish::parse
{


// Expressions
struct SubscriptExpr;
struct FunctionCallExpr;
struct VarRefExpr;
struct BinaryExpr;
struct UnaryExpr;
struct BoolLiteralExpr;
struct CharLiteralExpr;
struct IntLiteralExpr;
struct FloatLiteralExpr;
struct StringLiteralExpr;

// Statements
struct IfStatement;
struct AssignmentStatement;
struct VariableDeclarationStatement;
struct ArithmeticAssignmentStatement;
struct ReturnStatement;
struct ForStatement;
struct WhileStatement;
struct DoWhileStatement;
struct ExpressionStatement;

// Root items
struct FunctionDeclaration;
struct FunctionDefinition;
struct SystemInclude;
struct StructDeclaration;


// Variants
using IExpression = std::variant<
    SubscriptExpr,
    FunctionCallExpr,
    VarRefExpr,
    BinaryExpr,
    UnaryExpr,
    BoolLiteralExpr,
    CharLiteralExpr,
    IntLiteralExpr,
    FloatLiteralExpr,
    StringLiteralExpr
>;
using IStatement = std::variant<
    IfStatement,
    AssignmentStatement,
    VariableDeclarationStatement,
    ArithmeticAssignmentStatement,
    ReturnStatement,
    ForStatement,
    WhileStatement,
    DoWhileStatement,
    ExpressionStatement
>;
using IForLoopInitializer = std::variant<
    IExpression,
    AssignmentStatement,
    VariableDeclarationStatement,
    ArithmeticAssignmentStatement
>;
using IRootItem = std::variant<
    VariableDeclarationStatement,
    FunctionDeclaration,
    FunctionDefinition,
    SystemInclude,
    StructDeclaration
>;

/*
================
BUILDING BLOCKS
================
*/

struct Identifier {
    std::string name;
};
struct TypeIdentifier {
    bool operator==(const TypeIdentifier& other) const = default;
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
struct SubscriptExpr {
    std::unique_ptr<IExpression> ptrExpression;
    std::unique_ptr<IExpression> idxExpression;
};
struct FunctionCallExpr {
    std::string functionName;
    std::vector<std::unique_ptr<IExpression>> params;
};
struct VarRefExpr {
    Identifier identifier;
};
struct BinaryExpr {
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    BinaryOperator oper;
};
struct UnaryExpr {
    UnaryOperator oper;
    std::unique_ptr<IExpression> expression;
};
struct BoolLiteralExpr {
    bool value;
};
struct CharLiteralExpr {
    char value;
};
struct IntLiteralExpr {
    int64_t value;
};
struct FloatLiteralExpr {
    double value;
};
struct StringLiteralExpr {
    std::string value;
};


/*
================
STATEMENTS
================
*/
struct IfStatement {
    IExpression condition;
    std::vector<std::unique_ptr<IStatement>> body;
};
struct AssignmentStatement {
    IExpression left;
    IExpression right;
};
struct VariableDeclarationStatement {
    TypeIdentifier type;
    Identifier varName;
    std::optional<IExpression> expression;
};
struct ArithmeticAssignmentStatement {
    IExpression left;
    ArithmeticAssignmentOperator oper;
    IExpression right;
};
struct ReturnStatement {
    std::optional<IExpression> expression;
};
struct ForStatement {
    std::optional<IForLoopInitializer> initializer;
    std::optional<IExpression> condition;
    std::optional<IExpression> update;
};
struct WhileStatement {
    IExpression condition;
    std::vector<std::unique_ptr<IStatement>> body;
};
struct DoWhileStatement {
    IExpression condition;
    std::vector<std::unique_ptr<IStatement>> body;
};
struct ExpressionStatement {
    IExpression expression;
};

/*
================
ROOT ITEMS
================
*/
struct FunctionDeclaration {
    TypeIdentifier returnType;
    Identifier name;
    std::vector<FunctionParameter> params;
};
struct FunctionDefinition {
    FunctionDeclaration declaration;
    std::vector<std::unique_ptr<IStatement>> body;
};
struct SystemInclude {
    std::string moduleName;
};
struct StructDeclaration {
    Identifier name;
    std::vector<StructFieldDeclaration> fields;
};

struct ParseTree {
    std::vector<IRootItem> rootItems;
};


}
