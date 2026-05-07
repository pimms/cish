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

bool operator==(const SubscriptExpr& lhs, const SubscriptExpr& rhs);
bool operator==(const FunctionCallExpr& lhs, const FunctionCallExpr& rhs);
bool operator==(const VarRefExpr& lhs, const VarRefExpr& rhs);
bool operator==(const BinaryExpr& lhs, const BinaryExpr& rhs);
bool operator==(const UnaryExpr& lhs, const UnaryExpr& rhs);
bool operator==(const BoolLiteralExpr& lhs, const BoolLiteralExpr& rhs);
bool operator==(const CharLiteralExpr& lhs, const CharLiteralExpr& rhs);
bool operator==(const IntLiteralExpr& lhs, const IntLiteralExpr& rhs);
bool operator==(const FloatLiteralExpr& lhs, const FloatLiteralExpr& rhs);
bool operator==(const StringLiteralExpr& lhs, const StringLiteralExpr& rhs);

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
struct TypeIdentifier {
    bool operator==(const TypeIdentifier& other) const = default;
    bool isConst {};
    bool isStruct {};
    std::string type;
    // The number of *'s in the type. E.g., for "char**", pointerLevel is 2.
    int pointerLevel {};
};
struct FunctionParameter {
    bool operator==(const FunctionParameter&) const = default;
    TypeIdentifier type;
    std::optional<std::string> name;
};
struct StructFieldDeclaration {
    TypeIdentifier type;
    std::string name;
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
    std::string identifier;
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
    bool operator==(const VariableDeclarationStatement&) const = default;
    TypeIdentifier type;
    std::string varName;
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
    bool operator==(const FunctionDeclaration&) const = default;
    TypeIdentifier returnType;
    std::string name;
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
    std::string name;
    std::vector<StructFieldDeclaration> fields;
};

struct ParseTree {
    std::vector<IRootItem> rootItems;
};


}
