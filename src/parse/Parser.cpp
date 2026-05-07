#include "Parser.h"
#include "ParseTree.h"
#include "../Exception.h"

#include <cassert>
#include <cstddef>
#include <optional>

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
            return convertSystemInclude(*_context.take());
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
                .expression = std::nullopt
           };
        }
        case tok::TokenType::EQUAL: {
            _context.take();
            auto expression = parseExpression();
            if (!expression.has_value()) {
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

std::optional<SystemInclude> Parser::convertSystemInclude(const tok::Token& token)
{
    if (token.getType() != tok::TokenType::INCLUDE_SYS) {
        return std::nullopt;
    }

    auto it = token.getLexeme().begin();
    while (*it++ != '<') { }
    auto last = token.getLexeme().end() - 1;
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

std::optional<IExpression> Parser::parseExpression()
{
    Throw(ParseError, "TODO!")
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
