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
    std::optional<SystemInclude> convertSystemInclude(const tok::Token&);
    std::optional<StructDeclaration> parseStructDeclaration();

    std::unique_ptr<IStatement> parseStatement();

    std::optional<IExpression> parseExpression();
    std::optional<CharLiteralExpr> parseCharLiteral();
    std::optional<IntLiteralExpr> parseIntLiteralExpr();
    std::optional<FloatLiteralExpr> parseFloatLiteralExpr();
    std::optional<StringLiteralExpr> parseStringLiteralExpr();

    std::optional<UnaryOperator> parsePrefixUnaryOperator();
    std::optional<UnaryOperator> parsePostfixUnaryOperator();

    std::optional<TypeIdentifier> parseTypeIdentifier();
    std::optional<FunctionParameter> parseFunctionParameter();
};

}
