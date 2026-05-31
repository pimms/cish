#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "CodeMarker.h"

namespace cish::parse
{

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

    std::string toString() const;
};
struct FunctionParameter {
    bool operator==(const FunctionParameter&) const = default;
    TypeIdentifier type;
    std::optional<std::string> name;
};
struct StructFieldDeclaration {
    CodeInterval interval;
    TypeIdentifier type;
    std::string name;
};

// Expressions fwd
struct SubscriptExpr;
struct FunctionCallExpr;
struct VarRefExpr;
struct BinaryExpr;
struct TypeCastExpr;
struct UnaryExpr;
struct BoolLiteralExpr;
struct CharLiteralExpr;
struct IntLiteralExpr;
struct FloatLiteralExpr;
struct StringLiteralExpr;
struct MemberAccessExpr;
struct SizeofExpr;

// Statements fwd
struct IfStatement;
struct VariableDeclarationStatement;
struct ReturnStatement;
struct ForStatement;
struct WhileStatement;
struct DoWhileStatement;
struct ExpressionStatement;
struct ScopeStatement;

// Root items fwd
struct FunctionDeclaration;
struct FunctionDefinition;
struct SystemInclude;
struct StructDeclaration;

// Variants
using IExpression = std::variant<
    SubscriptExpr,
    FunctionCallExpr,
    BinaryExpr,
    TypeCastExpr,
    UnaryExpr,
    VarRefExpr,
    BoolLiteralExpr,
    CharLiteralExpr,
    IntLiteralExpr,
    FloatLiteralExpr,
    StringLiteralExpr,
    MemberAccessExpr,
    SizeofExpr
>;
using IStatement = std::variant<
    IfStatement,
    VariableDeclarationStatement,
    ReturnStatement,
    ForStatement,
    WhileStatement,
    DoWhileStatement,
    ExpressionStatement,
    ScopeStatement
>;
using IForLoopInitializer = std::variant<
    std::unique_ptr<IExpression>,
    std::unique_ptr<IStatement>
>;
using ISizeofTerm = std::variant<
    std::unique_ptr<IExpression>,
    TypeIdentifier
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
enum class MemberAccessOperator {
    DOT,
    ARROW,
};
enum class BinaryOperator {
    MULT,       DIVIDE,     MODULO,
    PLUS,       MINUS,
    LSHIFT,     RSHIFT,
    GTE,        LTE,        GT,         LT,
    EQUALS,     NEQUALS,
    BITAND,     BITXOR,     BITOR,
    LOGAND,     LOGOR,

    ASSIGN,
    ASS_MULT,       ASS_DIVIDE,     ASS_MODULO,
    ASS_PLUS,       ASS_MINUS,
    ASS_LSHIFT,     ASS_RSHIFT,
    ASS_BITAND,     ASS_BITXOR,     ASS_BITOR,
};
struct SubscriptExpr {
    CodeInterval interval;
    std::unique_ptr<IExpression> ptrExpression;
    std::unique_ptr<IExpression> idxExpression;
};
struct FunctionCallExpr {
    CodeInterval interval;
    std::string functionName;
    std::vector<std::unique_ptr<IExpression>> params;
};
struct VarRefExpr {
    CodeInterval interval;
    std::string identifier;
};
struct BinaryExpr {
    CodeInterval interval;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    BinaryOperator oper;
};
struct TypeCastExpr {
    CodeInterval interval;
    TypeIdentifier type;
    std::unique_ptr<IExpression> expr;
};
struct UnaryExpr {
    CodeInterval interval;
    UnaryOperator oper;
    std::unique_ptr<IExpression> expr;
};
struct BoolLiteralExpr {
    CodeInterval interval;
    bool value;
};
struct CharLiteralExpr {
    CodeInterval interval;
    // TODO: Use union or similar to properly handle signedness
    uint8_t value;
};
struct IntLiteralExpr {
    CodeInterval interval;
    // TODO: Use union or similar to properly handle signedness
    int64_t value;
};
struct FloatLiteralExpr {
    CodeInterval interval;
    double value;
};
struct StringLiteralExpr {
    CodeInterval interval;
    std::string value;
};
struct MemberAccessExpr {
    CodeInterval interval;
    std::unique_ptr<IExpression> expr;
    std::string member;
    MemberAccessOperator oper;
};
struct SizeofExpr {
    CodeInterval interval;
    ISizeofTerm term;
};

/*
================
STATEMENTS
================
*/
struct IfStatement {
    CodeInterval interval;
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<IStatement> positiveBody;
    std::unique_ptr<IStatement> negativeBody;
};
struct VariableDeclarationStatement {
    bool operator==(const VariableDeclarationStatement&) const = default;
    CodeInterval interval;
    TypeIdentifier type;
    std::string varName;
    std::unique_ptr<IExpression> expression;
};
struct ReturnStatement {
    CodeInterval interval;
    std::unique_ptr<IExpression> expression;
};
struct ForStatement {
    CodeInterval interval;
    std::unique_ptr<IForLoopInitializer> initializer;
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<IExpression> update;
    std::unique_ptr<IStatement> body;
};
struct WhileStatement {
    CodeInterval interval;
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<IStatement> body;
};
struct DoWhileStatement {
    CodeInterval interval;
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<IStatement> body;
};
struct ExpressionStatement {
    CodeInterval interval;
    std::unique_ptr<IExpression> expression;
};
struct ScopeStatement {
    CodeInterval interval;
    std::vector<std::unique_ptr<IStatement>> body;
};

/*
================
ROOT ITEMS
================
*/
struct FunctionDeclaration {
    bool operator==(const FunctionDeclaration&) const = default;
    CodeInterval interval;
    TypeIdentifier returnType;
    std::string name;
    std::vector<FunctionParameter> params;
};
struct FunctionDefinition {
    CodeInterval interval;
    FunctionDeclaration declaration;
    std::vector<std::unique_ptr<IStatement>> body;
};
struct SystemInclude {
    CodeInterval interval;
    std::string moduleName;
};
struct StructDeclaration {
    CodeInterval interval;
    std::string name;
    std::vector<StructFieldDeclaration> fields;
};

struct ParseTree {
    std::vector<IRootItem> rootItems;
};


}
