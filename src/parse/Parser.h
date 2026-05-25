#pragma once

#include "../tok/Token.h"
#include "TokenContext.h"
#include "ParseTree.h"

namespace cish::parse
{

namespace internal
{
using BinaryPrecedence = int;
const BinaryPrecedence BP_NONE = 0;
const BinaryPrecedence BP_PREFIX = 13;
std::optional<BinaryOperator> binaryOperatorFromToken(const tok::TokenType& type);
BinaryPrecedence binaryPrecedenceValue(BinaryOperator type);
}

class Parser {
public:
    explicit Parser(std::vector<tok::Token>& tokens);
    ~Parser() = default;
    Parser() = delete;
    Parser(const Parser& o) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::unique_ptr<ParseTree> parse();

private:
    TokenContext _context;

    void reset();

    std::optional<IRootItem> parseRootItem();
    std::optional<SystemInclude> parseSystemInclude();
    std::optional<StructDeclaration> parseStructDeclaration();

    std::unique_ptr<IStatement> parseStatement();
    std::unique_ptr<IStatement> parseIfStatement();
    std::unique_ptr<IStatement> parseReturnStatement();
    std::unique_ptr<IStatement> parseForStatement();
    std::unique_ptr<IForLoopInitializer> parseForLoopInitializer();
    std::unique_ptr<IStatement> parseWhileStatement();
    std::unique_ptr<IStatement> parseDoWhileStatement();
    std::unique_ptr<IStatement> parseVariableDeclarationStatement();

    std::unique_ptr<IStatement> parseScope();

    std::unique_ptr<IExpression> parseExpression(internal::BinaryPrecedence minBP);
    std::unique_ptr<IExpression> parseExpressionAtom();
    std::unique_ptr<IExpression> parseFunctionCallExpr();
    std::unique_ptr<IExpression> parseVarRefExpr();
    std::unique_ptr<IExpression> parseCharLiteralExpr();
    std::unique_ptr<IExpression> parseIntLiteralExpr();
    std::unique_ptr<IExpression> parseFloatLiteralExpr();
    std::unique_ptr<IExpression> parseStringLiteralExpr();
    std::optional<ISizeofTerm> parseSizeofTerm();

    std::optional<UnaryOperator> parsePrefixUnaryOperator();
    std::optional<BinaryOperator> parseBinaryOperator();
    std::optional<UnaryOperator> parsePostfixUnaryOperator();

    std::optional<TypeIdentifier> parseTypeCastOperator();
    std::optional<TypeIdentifier> parseTypeIdentifier();
    std::optional<FunctionParameter> parseFunctionParameter();
};

}
