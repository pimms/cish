#include "ParseTree.h"

namespace cish::parse
{

bool operator==(const SubscriptExpr& lhs, const SubscriptExpr& rhs) { return true; }
bool operator==(const FunctionCallExpr& lhs, const FunctionCallExpr& rhs) { return true; }
bool operator==(const VarRefExpr& lhs, const VarRefExpr& rhs) { return true; }
bool operator==(const BinaryExpr& lhs, const BinaryExpr& rhs) { return true; }
bool operator==(const UnaryExpr& lhs, const UnaryExpr& rhs) { return true; }
bool operator==(const BoolLiteralExpr& lhs, const BoolLiteralExpr& rhs) { return true; }
bool operator==(const CharLiteralExpr& lhs, const CharLiteralExpr& rhs) { return true; }
bool operator==(const IntLiteralExpr& lhs, const IntLiteralExpr& rhs) { return true; }
bool operator==(const FloatLiteralExpr& lhs, const FloatLiteralExpr& rhs) { return true; }
bool operator==(const StringLiteralExpr& lhs, const StringLiteralExpr& rhs) { return true; }

bool operator==(const std::unique_ptr<IExpression>& left, const std::unique_ptr<IExpression>& right)
{
    if (left == nullptr && right == nullptr) return true;
    if (!left || !right) return false;
    return (*left == *right);
}

}
