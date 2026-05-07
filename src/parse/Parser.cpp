#include "Parser.h"
#include "ParseTree.h"
#include "../Exception.h"

#include <cassert>
#include <optional>
#include <map>

namespace cish::parse
{

DECLARE_EXCEPTION(ParseError);

Parser::Parser(std::vector<tok::Token>& tokens)
    : _context(tokens)
{ }

ParseTree Parser::parse()
{
    reset();

    std::vector<IRootItem> rootItems;

    while (!_context.atEnd()) {
        auto rootItem = parseRootItem();
        if (!rootItem.has_value()) {
            Throw(ParseError, "Failed to parse root item for token:\n'%s'", _context.peek()->toString().c_str());
        }
        rootItems.push_back(std::move(rootItem).value());
    }

    return ParseTree {
        .rootItems = std::move(rootItems)
    };
}

void Parser::reset()
{
    _context.reset();
}

std::optional<IRootItem> Parser::parseRootItem()
{
    _context.exhaustSemicolons();
    switch (_context.peek()->getType()) {
        case tok::TokenType::END_OF_FILE:
            return std::nullopt;
        case tok::TokenType::INCLUDE_SYS:
            return convertSystemInclude();
        case tok::TokenType::STRUCT:
            return parseStructDeclaration();
        default:
            break;
    }

    // We now deal with one of the following:
    //  - global variable declaration
    //  - function declaration
    //  - function definition
    //
    // All of them begin with a type identifier, so we can start looking for that.
    auto typeIdentifier = parseTypeIdentifier();
    if (!typeIdentifier.has_value()) {
        Throw(ParseError, "Expected type identifier, found %s", _context.peek()->toString().c_str());
    }

    auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!identifier) {
        Throw(ParseError, "Expected identifier, found %s", _context.peek()->toString().c_str());
    }

    // If we now encounter either a semicolon or an equal sign, we know it's
    // a variable.
    switch (_context.peek()->getType()) {
        case tok::TokenType::SEMICOLON: {
            _context.take();
            return VariableDeclarationStatement {
                .type = typeIdentifier.value(),
                .varName = identifier->getLexeme(),
                .expression = nullptr
           };
        }
        case tok::TokenType::EQUAL: {
            _context.take();
            auto expression = parseExpression();
            if (!expression) {
                Throw(ParseError, "Expected expression");
            }
            if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
                Throw(ParseError, "Expected semicolon");
            }
            return VariableDeclarationStatement {
                .type = typeIdentifier.value(),
                .varName = identifier->getLexeme(),
                .expression = std::move(expression)
            };
        }
        default:
            break;
    }

    // We now know that it's either a func decl or def.
    if (!_context.takeIf(tok::TokenType::PAREN_L)) {
        Throw(ParseError, "Expected '(', found %s", _context.peek()->toString().c_str());
    }

    std::vector<FunctionParameter> params;
    std::optional<FunctionParameter> param = parseFunctionParameter();
    if (param.has_value()) {
        params.push_back(param.value());
        while (_context.takeIf(tok::TokenType::COMMA)) {
            param = parseFunctionParameter();
            if (!param.has_value()) {
                Throw(ParseError, "Expected parameter, found %s", _context.peek()->toString().c_str());
            }
            params.push_back(param.value());
        }
    }

    if (!_context.takeIf(tok::TokenType::PAREN_R)) {
        Throw(ParseError, "Expected ')', found %s", _context.peek()->toString().c_str());
    }

    FunctionDeclaration fdecl = FunctionDeclaration {
        .returnType = typeIdentifier.value(),
        .name = identifier->getLexeme(),
        .params = params
    };

    if (_context.takeIf(tok::TokenType::SEMICOLON)) {
        return fdecl;
    }

    if (!_context.takeIf(tok::TokenType::CBRACE_L)) {
        Throw(ParseError, "Expected '{', found %s", _context.peek()->toString().c_str());
    }
    std::vector<std::unique_ptr<IStatement>> statements;
    std::unique_ptr<IStatement> statement = parseStatement();
    while (statement) {
        statements.push_back(std::move(statement));
    }
    if (!_context.takeIf(tok::TokenType::CBRACE_R)) {
        Throw(ParseError, "Expected '}', found %s", _context.peek()->toString().c_str());
    }

    return FunctionDefinition {
        .declaration = fdecl,
        .body = std::move(statements)
    };
}

std::optional<SystemInclude> Parser::convertSystemInclude()
{
    const auto token = _context.takeIf(tok::TokenType::INCREMENT);
    if (!token) {
        return std::nullopt;
    }

    auto it = token->getLexeme().begin();
    while (*it++ != '<') { }
    auto last = token->getLexeme().end() - 1;
    std::string moduleName(it, last);
    if (moduleName.empty()) {
        return std::nullopt;
    }

    return SystemInclude {
        .moduleName = moduleName
    };
}

std::optional<StructDeclaration> Parser::parseStructDeclaration()
{
    auto tokenTransaction = _context.beginTransaction();

    if (!_context.takeIf(tok::TokenType::STRUCT)) {
        return std::nullopt;
    }

    auto structIdentifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!structIdentifier) {
        Throw(ParseError, "Expected identifier, found '%s'", _context.peek()->toString().c_str());
    }

    if (!_context.takeIf(tok::TokenType::CBRACE_L)) {
        Throw(ParseError, "Expected '{', found '%s'", _context.peek()->toString().c_str());
    }

    std::vector<StructFieldDeclaration> fields;
    while (!_context.atEnd() && _context.peek()->getType() != tok::TokenType::CBRACE_R) {
        _context.exhaustSemicolons();
        auto typeIdentifier = parseTypeIdentifier();
        if (!typeIdentifier.has_value()) {
            Throw(ParseError, "Expected type identifier, found %s", _context.peek()->toString().c_str());
        }

        auto fieldIdentifier = _context.takeIf(tok::TokenType::IDENTIFIER);
        if (!fieldIdentifier) {
            Throw(ParseError, "Expected identifier");
        }

        if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
            Throw(ParseError, "Expected semicolon");
        }

        StructFieldDeclaration field = {
            .type = typeIdentifier.value(),
            .name = fieldIdentifier->getLexeme()
        };
        fields.push_back(field);
    }

    if (!_context.takeIf(tok::TokenType::CBRACE_R) || !_context.takeIf(tok::TokenType::SEMICOLON)) {
        if (!_context.atEnd()) {
            Throw(ParseError, "Expected '};', found '%s'", _context.peek()->toString().c_str());
        }
        Throw(ParseError, "Expected '};', found EOF");
    }

    tokenTransaction.commit();

    return StructDeclaration {
        .name = structIdentifier->getLexeme(),
        .fields = std::move(fields)
    };
}

std::unique_ptr<IStatement> Parser::parseStatement()
{
    Throw(ParseError, "TODO");
}

std::unique_ptr<IExpression> Parser::parseExpression()
{
    std::optional<UnaryOperator> unary = parsePrefixUnaryOperator();


    Throw(ParseError, "TODO!")
}

std::unique_ptr<FunctionCallExpr> Parser::parseFunctionCallExpr()
{
    auto transaction = _context.beginTransaction();

    auto functionName = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!functionName) return nullptr;

    if (!_context.takeIf(tok::TokenType::PAREN_L)) return nullptr;

    std::vector<std::unique_ptr<IExpression>> params;
    std::unique_ptr<IExpression> p;

    bool expectParam = false;

    while (!_context.takeIf(tok::TokenType::PAREN_R)) {
        auto expr = parseExpression();
        if (!expr) {
            Throw(ParseError, "Unable to parse function parameter");
        }
        expectParam = false;
        if (_context.takeIf(tok::TokenType::COMMA)) {
            expectParam = true;
        }
    }
    if (expectParam) {
        Throw(ParseError, "Expected parameter, found ')'");
    }

    if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
        Throw(ParseError, "Expected ';', found %s", _context.peek()->toString().c_str());
    }

    transaction.commit();
    return std::make_unique<FunctionCallExpr>(
        functionName->getLexeme(),
        std::move(params)
    );
}

std::unique_ptr<CharLiteralExpr> Parser::parseCharLiteral()
{
    const auto& token = _context.takeIf(tok::TokenType::LIT_CHAR);
    if (!token) {
        return nullptr;
    }

    const auto& lexeme = token->getLexeme();
    assert(lexeme.size() > 2);

    const std::string literal = lexeme.substr(1, lexeme.size() - 2);
    if (literal == "\\") {
        Throw(ParseError, "Literal '\\' is not a valid char, in token %s", token->toString().c_str());
    }

    uint8_t value = 0;
    if (literal[0] == '\\') {
        assert(literal.size() == 2);
        switch (literal[1]) {
            case '0': value = '\0'; break;
            case 'a': value = '\a'; break;
            case 'b': value = '\b'; break;
            case 't': value = '\t'; break;
            case 'n': value = '\n'; break;
            case 'v': value = '\v'; break;
            case 'f': value = '\f'; break;
            case 'r': value = '\r'; break;
            case 'e': value = '\e'; break;
            case '\\': value = '\\'; break;
            default: Throw(ParseError, "Unknown escape sequence: %s", token->toString().c_str())
        }
    } else {
        assert(literal.size() == 1);
        value = literal[0];
    }

    return std::make_unique<CharLiteralExpr>(value);
}

std::unique_ptr<IntLiteralExpr> Parser::parseIntLiteralExpr()
{
    const auto& token = _context.takeIf(tok::TokenType::LIT_INT);
    if (!token) {
        return nullptr;
    }

    int64_t value = 0;
    size_t parsed = 0;

    const auto& lexeme = token->getLexeme();
    if (lexeme.starts_with("0x") || lexeme.starts_with("0X")) {
        value = std::stoi(lexeme.c_str()+2, &parsed, 16);
        parsed += 2;
    } else if (lexeme.starts_with("0b") || lexeme.starts_with("0B")) {
        value = std::stoi(lexeme.c_str()+2, &parsed, 2);
        parsed += 2;
    } else if (lexeme.starts_with("0")) {
        value = std::stoi(lexeme.c_str()+1, &parsed, 8);
        parsed += 1;
    } else {
        value = std::stoi(lexeme, &parsed, 10);
    }

    if (parsed != lexeme.size()) {
        Throw(ParseError, "Unexpected characters in int literal: %s", token->toString().c_str());
    }

    return std::make_unique<IntLiteralExpr>(value);
}

std::unique_ptr<FloatLiteralExpr> Parser::parseFloatLiteralExpr()
{
    const auto& token = _context.takeIf(tok::TokenType::LIT_FLOAT);
    if (!token) {
        return nullptr;
    }

    const auto& lexeme = token->getLexeme();

    size_t parsed = 0;
    const double value = std::stod(lexeme, &parsed);
    if (parsed < lexeme.size()) {
        if (parsed+1 != lexeme.size() || lexeme[parsed] != 'f' || lexeme[parsed] != 'F') {
            Throw(ParseError, "Unexpected characters in float literal: %s", token->toString().c_str());
        }
    }

    return std::make_unique<FloatLiteralExpr>(value);
}

std::unique_ptr<StringLiteralExpr> Parser::parseStringLiteralExpr()
{
    const auto& token = _context.takeIf(tok::TokenType::LIT_STRING);
    if (!token) {
        return nullptr;
    }

    const auto& lexeme = token->getLexeme();
    assert(lexeme.size() >= 2);
    std::string value = lexeme.substr(1, lexeme.size() - 2);
    return std::make_unique<StringLiteralExpr>(value);
}

std::optional<UnaryOperator> Parser::parsePrefixUnaryOperator()
{
    switch (_context.peek()->getType()) {
        case tok::TokenType::INCREMENT: return UnaryOperator::PREINC;
        case tok::TokenType::DECREMENT: return UnaryOperator::PREDEC;
        case tok::TokenType::MINUS: return UnaryOperator::MINUS;
        case tok::TokenType::BANG: return UnaryOperator::NEGATE;
        case tok::TokenType::TILDE: return UnaryOperator::ONES_COMPL;
        case tok::TokenType::STAR: return UnaryOperator::DEREF;
        case tok::TokenType::AMPERSAND: return UnaryOperator::ADDROF;
        case tok::TokenType::SIZEOF: return UnaryOperator::SIZEOF;
        default: return std::nullopt;
    }
}

std::optional<UnaryOperator> Parser::parsePostfixUnaryOperator()
{
    switch (_context.peek()->getType()) {
        case tok::TokenType::INCREMENT: return UnaryOperator::POSTINC;
        case tok::TokenType::DECREMENT: return UnaryOperator::POSTDEC;
        default: return std::nullopt;
    }
}

std::optional<TypeIdentifier> Parser::parseTypeIdentifier()
{
    auto transaction = _context.beginTransaction();

    bool isConst = false;
    bool isStruct = false;
    if (_context.takeIf(tok::TokenType::CONST)) {
        isConst = true;
    }

    if (_context.takeIf(tok::TokenType::STRUCT)) {
        isStruct = true;
    }

    auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!identifier) {
        return std::nullopt;
    }

    transaction.commit();

    int pointerLevel = 0;
    while (_context.takeIf(tok::TokenType::STAR)) {
        pointerLevel++;
    }

    return TypeIdentifier {
        .isConst = isConst,
        .isStruct = isStruct,
        .type = identifier->getLexeme(),
        .pointerLevel = pointerLevel
    };
}

std::optional<FunctionParameter> Parser::parseFunctionParameter()
{
    auto type = parseTypeIdentifier();
    if (!type.has_value()) {
        return std::nullopt;
    }

    auto idToken = _context.takeIf(tok::TokenType::IDENTIFIER);

    std::optional<std::string> identifier;
    if (idToken) {
        identifier = idToken->getLexeme();
    }

    return FunctionParameter {
        .type = type.value(),
        .name = identifier
    };
}

}
