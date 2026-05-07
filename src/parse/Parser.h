#pragma once

#include "../tok/Token.h"
#include "TokenContext.h"
#include "ParseTree.h"

namespace cish::parse
{

class Parser {
public:
    explicit Parser(std::vector<tok::Token>& tokens);
    Parser(const Parser& o) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;
    Parser& operator=(const Parser&) = delete;

    ParseTree parse();

private:
    TokenContext _context;

    void reset();

    std::optional<IRootItem> parseRootItem();
    std::optional<SystemInclude> convertSystemInclude();
    std::optional<StructDeclaration> parseStructDeclaration();

    std::unique_ptr<IStatement> parseStatement();

    std::unique_ptr<IExpression> parseExpression();
    std::unique_ptr<FunctionCallExpr> parseFunctionCallExpr();
    std::unique_ptr<CharLiteralExpr> parseCharLiteral();
    std::unique_ptr<IntLiteralExpr> parseIntLiteralExpr();
    std::unique_ptr<FloatLiteralExpr> parseFloatLiteralExpr();
    std::unique_ptr<StringLiteralExpr> parseStringLiteralExpr();

    std::optional<UnaryOperator> parsePrefixUnaryOperator();
    std::optional<UnaryOperator> parsePostfixUnaryOperator();

    std::optional<TypeIdentifier> parseTypeIdentifier();
    std::optional<FunctionParameter> parseFunctionParameter();
};

}
