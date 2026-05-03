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
    if (_context.peek()->getType() == tok::TokenType::INCLUDE_SYS) {
        return convertSystemInclude(*_context.take());
    } else if (_context.peek()->getType() == tok::TokenType::STRUCT) {
        return parseStructDeclaration();
    }

    Throw(ParseError, "Not supported yet: %s", _context.peek()->toString().c_str());
    return std::nullopt;
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
    if (moduleName.size() == 0) {
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

    auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!identifier) {
        Throw(ParseError, "Expected identifier, found '%s'", _context.peek()->toString().c_str());
        return std::nullopt;
    }

    if (!_context.takeIf(tok::TokenType::CBRACE_L)) {
        Throw(ParseError, "Expected '{', found '%s'", _context.peek()->toString().c_str());
        return std::nullopt;
    }

    std::vector<StructFieldDeclaration> fields;
    while (!_context.atEnd() && _context.peek()->getType() != tok::TokenType::CBRACE_R) {
        _context.exhaustSemicolons();

        auto typeIdentifier = parseTypeIdentifier();
        if (!typeIdentifier.has_value()) {
            Throw(ParseError, "Expected type identifier, found %s", _context.peek()->toString().c_str());
        }

        auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
        if (!identifier) {
            Throw(ParseError, "Expected identifier");
        }

        if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
            Throw(ParseError, "Expected semicolon");
        }

        StructFieldDeclaration field = {
            .type = typeIdentifier.value(),
            .name = Identifier { identifier->getLexeme() }
        };
        fields.push_back(field);
    }

    if (!_context.takeIf(tok::TokenType::CBRACE_R) || !_context.takeIf(tok::TokenType::SEMICOLON)) {
        if (!_context.atEnd()) {
            Throw(ParseError, "Expected '};', found '%s'", _context.peek()->toString().c_str());
        } else {
            Throw(ParseError, "Expected '};', found EOF");
        }
        return std::nullopt;
    }

    _context.exhaustSemicolons();
    tokenTransaction.commit();

    return StructDeclaration {
        .name = Identifier { identifier->getLexeme() },
        .fields = std::move(fields)
    };
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


}
