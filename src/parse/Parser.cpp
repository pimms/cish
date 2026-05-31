#include "Parser.h"
#include "ParseTree.h"
#include "../Exception.h"

#include <cassert>
#include <optional>
#include <map>
#include <iostream>

#include "../ast/ForLoopStatement.h"

#define DLOG(_C, _M)  \
    std::cout << "[" << #_C << "] " << _M << std::endl;

namespace cish::parse
{

DECLARE_EXCEPTION(ParseError);

namespace internal
{
DECLARE_EXCEPTION(InternalError);

std::optional<BinaryOperator> binaryOperatorFromToken(const tok::TokenType& type)
{
    switch (type) {
        case tok::TokenType::STAR: return BinaryOperator::MULT;
        case tok::TokenType::RSLASH: return BinaryOperator::DIVIDE;
        case tok::TokenType::MODULO: return BinaryOperator::MODULO;
        case tok::TokenType::PLUS: return BinaryOperator::PLUS;
        case tok::TokenType::MINUS: return BinaryOperator::MINUS;
        case tok::TokenType::LSHIFT: return BinaryOperator::LSHIFT;
        case tok::TokenType::RSHIFT: return BinaryOperator::RSHIFT;
        case tok::TokenType::CMP_EQ: return BinaryOperator::EQUALS;
        case tok::TokenType::CMP_NE: return BinaryOperator::NEQUALS;
        case tok::TokenType::CMP_GTEQ: return BinaryOperator::GTE;
        case tok::TokenType::CMP_LTEQ: return BinaryOperator::LTE;
        case tok::TokenType::ABRACE_L: return BinaryOperator::LT;
        case tok::TokenType::ABRACE_R: return BinaryOperator::GT;
        case tok::TokenType::AMPERSAND: return BinaryOperator::BITAND;
        case tok::TokenType::CARET: return BinaryOperator::BITXOR;
        case tok::TokenType::PIPE: return BinaryOperator::BITOR;
        case tok::TokenType::LOG_AND: return BinaryOperator::LOGAND;
        case tok::TokenType::LOG_OR: return BinaryOperator::LOGOR;
        case tok::TokenType::EQUAL: return BinaryOperator::ASSIGN;
        case tok::TokenType::MUL_ASSIGN: return BinaryOperator::ASS_MULT;
        case tok::TokenType::DIV_ASSIGN: return BinaryOperator::ASS_DIVIDE;
        case tok::TokenType::MOD_ASSIGN: return BinaryOperator::ASS_MODULO;
        case tok::TokenType::PLUS_ASSIGN: return BinaryOperator::ASS_PLUS;
        case tok::TokenType::MINUS_ASSIGN: return BinaryOperator::ASS_MINUS;
        case tok::TokenType::LS_ASSIGN: return BinaryOperator::ASS_LSHIFT;
        case tok::TokenType::RS_ASSIGN: return BinaryOperator::ASS_RSHIFT;
        case tok::TokenType::BWAND_ASSIGN: return BinaryOperator::ASS_BITAND;
        case tok::TokenType::BXOR_ASSIGN: return BinaryOperator::ASS_BITXOR;
        case tok::TokenType::BWOR_ASSIGN: return BinaryOperator::ASS_BITOR;
        default: return std::nullopt;
    }
}

BinaryPrecedence binaryPrecedenceValue(BinaryOperator type)
{
    switch (type) {
        // = += -= *= etc.  (right-associative)
        case BinaryOperator::ASSIGN:
        case BinaryOperator::ASS_MULT:
        case BinaryOperator::ASS_DIVIDE:
        case BinaryOperator::ASS_MODULO:
        case BinaryOperator::ASS_PLUS:
        case BinaryOperator::ASS_MINUS:
        case BinaryOperator::ASS_LSHIFT:
        case BinaryOperator::ASS_RSHIFT:
        case BinaryOperator::ASS_BITAND:
        case BinaryOperator::ASS_BITXOR:
        case BinaryOperator::ASS_BITOR:
            return 1;

        case BinaryOperator::LOGOR:
            return 2;
        case BinaryOperator::LOGAND:
            return 3;
        case BinaryOperator::BITOR:
            return 4;
        case BinaryOperator::BITXOR:
            return 5;
        case BinaryOperator::BITAND:
            return 6;
        case BinaryOperator::EQUALS:
        case BinaryOperator::NEQUALS:
            return 7;
        case BinaryOperator::LTE:
        case BinaryOperator::GTE:
        case BinaryOperator::LT:
        case BinaryOperator::GT:
            return 8;
        case BinaryOperator::LSHIFT:
        case BinaryOperator::RSHIFT:
            return 9;
        case BinaryOperator::PLUS:
        case BinaryOperator::MINUS:
            return 10;
        case BinaryOperator::MULT:
        case BinaryOperator::DIVIDE:
        case BinaryOperator::MODULO:
            return 11;
        default:
            Throw(InternalError, "Unhandled operator: %d", type);
    }
}

bool isAssignmentOperator(BinaryOperator op)
{
    switch (op) {
        case BinaryOperator::ASSIGN:
        case BinaryOperator::ASS_MULT:
        case BinaryOperator::ASS_DIVIDE:
        case BinaryOperator::ASS_MODULO:
        case BinaryOperator::ASS_PLUS:
        case BinaryOperator::ASS_MINUS:
        case BinaryOperator::ASS_LSHIFT:
        case BinaryOperator::ASS_RSHIFT:
        case BinaryOperator::ASS_BITAND:
        case BinaryOperator::ASS_BITXOR:
        case BinaryOperator::ASS_BITOR:
            return true;
        default:
            return false;
    }
}
}

using namespace internal;

Parser::Parser(std::vector<tok::Token>& tokens)
    : _context(tokens)
{ }

std::unique_ptr<ParseTree> Parser::parse()
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

    return std::make_unique<ParseTree>(std::move(rootItems));
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
            DLOG(ROOT, "reached EOF");
            return std::nullopt;
        case tok::TokenType::INCLUDE_SYS:
            DLOG(ROOT, "parsing sys include");
            return parseSystemInclude();
        case tok::TokenType::STRUCT:
            DLOG(ROOT, "parsing struct decl (attempt)");
            if (auto structDecl = parseStructDeclaration()) {
                DLOG(ROOT, "parsed struct decl");
                return structDecl;
            }
            DLOG(ROOT, "struct decl attempt failed");
        default:
            break;
    }

    // We now deal with one of the following:
    //  - global variable declaration
    //  - function declaration
    //  - function definition
    //
    // All of them begin with a type identifier, so we can start looking for that.
    auto interval = _context.getIntervalReader();
    auto typeIdentifier = parseTypeIdentifier();
    if (!typeIdentifier.has_value()) {
        Throw(ParseError, "Expected type identifier, found %s", _context.peek()->toString().c_str());
    }

    auto identifier = _context.require(tok::TokenType::IDENTIFIER);

    // If we now encounter either a semicolon or an equal sign, we know it's
    // a variable.
    switch (_context.peek()->getType()) {
        case tok::TokenType::SEMICOLON: {
            DLOG(ROOT, "parsed variable decl");
            _context.take();
            return VariableDeclarationStatement {
                interval.getInterval(),
                typeIdentifier.value(),
                identifier->getLexeme(),
                nullptr
           };
        }
        case tok::TokenType::EQUAL: {
            _context.take();
            auto expression = parseExpression(BP_NONE);
            if (!expression) {
                Throw(ParseError, "Expected expression");
            }
            _context.require(tok::TokenType::SEMICOLON);
            DLOG(ROOT, "parsed variable decl w assign");
            return VariableDeclarationStatement {
                interval.getInterval(),
                typeIdentifier.value(),
                identifier->getLexeme(),
                std::move(expression)
            };
        }
        default:
            break;
    }

    // We now know that it's either a func decl or def.
    _context.require(tok::TokenType::PAREN_L);

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

    _context.require(tok::TokenType::PAREN_R);

    FunctionDeclaration fdecl = FunctionDeclaration {
        interval.getInterval(),
        typeIdentifier.value(),
        identifier->getLexeme(),
        params
    };

    if (_context.takeIf(tok::TokenType::SEMICOLON)) {
        DLOG(ROOT, "parsed function decl");
        return fdecl;
    }

    DLOG(ROOT, "parsing function def");
    _context.require(tok::TokenType::CBRACE_L);
    std::vector<std::unique_ptr<IStatement>> statements;
    while (auto statement = parseStatement()) {
        statements.push_back(std::move(statement));
    }
    _context.require(tok::TokenType::CBRACE_R);

    DLOG(ROOT, "parsed function def");

    return FunctionDefinition {
        interval.getInterval(),
        fdecl,
        std::move(statements)
    };
}

std::optional<SystemInclude> Parser::parseSystemInclude()
{
    auto interval = _context.getIntervalReader();
    const auto token = _context.takeIf(tok::TokenType::INCLUDE_SYS);
    if (!token) {
        return std::nullopt;
    }

    auto it = token->getLexeme().begin();
    while (*it++ != '<') { }
    const auto last = token->getLexeme().end() - 1;
    const std::string moduleName(it, last);
    if (moduleName.empty()) {
        return std::nullopt;
    }

    return SystemInclude {
        interval.getInterval(),
        moduleName
    };
}

std::optional<StructDeclaration> Parser::parseStructDeclaration()
{
    auto tokenTransaction = _context.beginTransaction();

    if (!_context.takeIf(tok::TokenType::STRUCT)) {
        return std::nullopt;
    }

    auto structIdentifier = _context.require(tok::TokenType::IDENTIFIER);

    if (!_context.takeIf(tok::TokenType::CBRACE_L)) {
        // This is likely a function returning a struct, not a declaration.
        return std::nullopt;
    }

    std::vector<StructFieldDeclaration> fields;
    while (!_context.atEnd() && _context.peek()->getType() != tok::TokenType::CBRACE_R) {
        _context.exhaustSemicolons();

        auto interval = _context.getIntervalReader();
        auto typeIdentifier = parseTypeIdentifier();
        if (!typeIdentifier.has_value()) {
            Throw(ParseError, "Expected type identifier, found %s", _context.peek()->toString().c_str());
        }

        auto fieldIdentifier = _context.require(tok::TokenType::IDENTIFIER);
        _context.require(tok::TokenType::SEMICOLON);

        StructFieldDeclaration field = {
            .interval = interval.getInterval(),
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
    switch (_context.peek()->getType()) {
        case tok::TokenType::IF:
            DLOG(STMT, "Parsing if statement");
            return parseIfStatement();
        case tok::TokenType::RETURN:
            DLOG(STMT, "Parsing return statement");
            return parseReturnStatement();
        case tok::TokenType::FOR:
            DLOG(STMT, "Parsing for statement");
            return parseForStatement();
        case tok::TokenType::WHILE:
            DLOG(STMT, "Parsing while statement");
            return parseWhileStatement();
        case tok::TokenType::DO:
            DLOG(STMT, "Parsing do-while statement");
            return parseDoWhileStatement();
        case tok::TokenType::CBRACE_L:
            DLOG(STMT, "Parsing scope statement");
            return parseScope();
        default:
            break;
    }

    if (auto varDecl = parseVariableDeclarationStatement()) {
        DLOG(STMT, "parsed variable declaration statement");
        return varDecl;
    }
    auto interval = _context.getIntervalReader();
    if (auto expr = parseExpression(BP_NONE)) {
        _context.require(tok::TokenType::SEMICOLON);
        DLOG(STMT, "parsed expression statement");
        return std::make_unique<IStatement>(
            ExpressionStatement(interval.getInterval(), std::move(expr))
        );
    }

    DLOG(STMT, "failed to parse statement");
    return nullptr;
}

std::unique_ptr<IStatement> Parser::parseIfStatement()
{
    auto interval = _context.getIntervalReader();
    _context.require(tok::TokenType::IF);
    _context.require(tok::TokenType::PAREN_L);
    std::unique_ptr<IExpression> condition = parseExpression(BP_NONE);
    _context.require(tok::TokenType::PAREN_R);
    std::unique_ptr<IStatement> trueScope = parseScope();

    std::unique_ptr<IStatement> falseScope = nullptr;
    if (_context.takeIf(tok::TokenType::ELSE)) {
        std::unique_ptr<IStatement> elseScope;
        if (_context.peek()->getType() == tok::TokenType::IF) {
            falseScope = parseIfStatement();
        } else {
            falseScope = parseScope();
        }
    }

    return std::make_unique<IStatement>(
        IfStatement(
            interval.getInterval(),
            std::move(condition),
            std::move(trueScope),
            std::move(falseScope)
        )
    );
}

std::unique_ptr<IStatement> Parser::parseReturnStatement()
{
    auto interval = _context.getIntervalReader();

    _context.require(tok::TokenType::RETURN);
    auto expr = parseExpression(BP_NONE);
    _context.require(tok::TokenType::SEMICOLON);

    return std::make_unique<IStatement>(ReturnStatement(
        interval.getInterval(),
        std::move(expr)
    ));
}

std::unique_ptr<IStatement> Parser::parseForStatement()
{
    auto interval = _context.getIntervalReader();

    _context.require(tok::TokenType::FOR);
    _context.require(tok::TokenType::PAREN_L);

    std::unique_ptr<IForLoopInitializer> init;
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<IExpression> update;

    if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
        init = parseForLoopInitializer();
    }
    if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
        condition = parseExpression(BP_NONE);
        _context.require(tok::TokenType::SEMICOLON);
    }
    update = parseExpression(BP_NONE);

    _context.require(tok::TokenType::PAREN_R);
    auto body = parseScope();

    return std::make_unique<IStatement>(
        ForStatement(
            interval.getInterval(),
            std::move(init),
            std::move(condition),
            std::move(update),
            std::move(body)
        )
    );
}

std::unique_ptr<IForLoopInitializer> Parser::parseForLoopInitializer()
{
    auto decl = parseVariableDeclarationStatement();
    if (decl) {
        return std::make_unique<IForLoopInitializer>(std::move(decl));
    }

    auto expression = parseExpression(BP_NONE);
    if (expression) {
        _context.require(tok::TokenType::SEMICOLON);
        return std::make_unique<IForLoopInitializer>(std::move(expression));
    }

    Throw(ParseError,
        "Expected expression, assignment or declaration in for loop initializer at line",
        _context.peek()->getLine());
}

std::unique_ptr<IStatement> Parser::parseWhileStatement()
{
    auto interval = _context.getIntervalReader();

    _context.require(tok::TokenType::WHILE);
    _context.require(tok::TokenType::PAREN_L);
    auto condition = parseExpression(BP_NONE);
    _context.require(tok::TokenType::PAREN_R);
    auto body = parseScope();

    return std::make_unique<IStatement>(
        WhileStatement(
            interval.getInterval(),
            std::move(condition),
            std::move(body)
        )
    );
}

std::unique_ptr<IStatement> Parser::parseDoWhileStatement()
{
    auto interval = _context.getIntervalReader();

    _context.require(::cish::tok::TokenType::DO);
    auto body = parseScope();
    _context.require(tok::TokenType::WHILE);
    _context.require(tok::TokenType::PAREN_L);
    auto condition = parseExpression(BP_NONE);
    _context.require(tok::TokenType::PAREN_R);
    _context.require(tok::TokenType::SEMICOLON);

    return std::make_unique<IStatement>(
        DoWhileStatement(
            interval.getInterval(),
            std::move(condition),
            std::move(body)
        )
    );
}

std::unique_ptr<IStatement> Parser::parseVariableDeclarationStatement()
{
    auto interval = _context.getIntervalReader();
    auto transaction = _context.beginTransaction();

    auto type = parseTypeIdentifier();
    if (!type.has_value()) {
        return nullptr;
    }

    auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!identifier) {
        return nullptr;
    }

    std::unique_ptr<IExpression> expr = nullptr;
    if (_context.takeIf(tok::TokenType::EQUAL)) {
        expr = parseExpression(BP_NONE);
        if (!expr) {
            return nullptr;
        }
    }


    if (!_context.takeIf(tok::TokenType::SEMICOLON)) {
        return nullptr;
    }

    transaction.commit();

    return std::make_unique<IStatement>(
        VariableDeclarationStatement(
            interval.getInterval(),
            type.value(),
            identifier->getLexeme(),
            std::move(expr)
        )
    );
}

std::unique_ptr<IStatement> Parser::parseScope()
{
    auto interval = _context.getIntervalReader();
    std::vector<std::unique_ptr<IStatement>> body;

    if (_context.takeIf(tok::TokenType::SEMICOLON)) {
        // Cool - we need support completely empty scopes for all loops.
        // Example:  while (true);
    } else if (_context.takeIf(tok::TokenType::CBRACE_L)) {
        while (!_context.takeIf(tok::TokenType::CBRACE_R)) {
            auto statement = parseStatement();
            body.push_back(std::move(statement));
        }
    } else {
        auto statement = parseStatement();
        if (!statement) {
            Throw(ParseError, "Expected statement, found %s", _context.peek()->toString().c_str());
        }
        body.push_back(std::move(statement));
    }

    return std::make_unique<IStatement>(ScopeStatement(
        interval.getInterval(),
        std::move(body)
    ));
}

std::unique_ptr<IExpression> Parser::parseExpression(BinaryPrecedence minBP)
{
    auto interval = _context.getIntervalReader();

    auto prefixOperator = parsePrefixUnaryOperator();
    std::unique_ptr<IExpression> left;

    if (prefixOperator.has_value()) {
        if (prefixOperator.value() == UnaryOperator::SIZEOF) {
            auto term = parseSizeofTerm();
            if (!term.has_value()) return nullptr;
            left = std::make_unique<IExpression>(SizeofExpr(
                interval.getInterval(),
                std::move(term.value())
            ));
        } else {
            auto operand = parseExpression(BP_PREFIX);
            if (!operand) return nullptr;
            left = std::make_unique<IExpression>(UnaryExpr(
                interval.getInterval(),
                prefixOperator.value(),
                std::move(operand)
            ));
        }
    } else if (auto typeCast = parseTypeCastOperator()) {
        auto operand = parseExpression(BP_PREFIX);
        if (!operand) return nullptr;
        left = std::make_unique<IExpression>(TypeCastExpr(
            interval.getInterval(),
            typeCast.value(),
            std::move(operand)
        ));
    } else {
        left = parseExpressionAtom();
    }

    if (!left) return nullptr;

    while (!_context.atEnd()) {
        // Handle subscript operators
        if (_context.peek()->getType() == tok::TokenType::SQPAREN_L) {
            _context.take();
            auto subscript = parseExpression(BP_NONE);
            if (!subscript) {
                Throw(ParseError, "Expected expression in subscript, found %s", _context.peek()->toString().c_str());
            }
            _context.require(tok::TokenType::SQPAREN_R);
            left = std::make_unique<IExpression>(SubscriptExpr(
                interval.getInterval(),
                std::move(left),
                std::move(subscript)
            ));
            continue;
        }

        // Handle member access operator
        if (_context.peek()->getType() == tok::TokenType::DOT || _context.peek()->getType() == tok::TokenType::ARROW) {
            auto memberInterval =_context.getIntervalReader();
            auto maToken = _context.take();
            auto memToken = _context.require(tok::TokenType::IDENTIFIER);
            left = std::make_unique<IExpression>(MemberAccessExpr(
                memberInterval.getInterval(),
                std::move(left),
                memToken->getLexeme(),
                (maToken->getType() == tok::TokenType::DOT ? MemberAccessOperator::DOT : MemberAccessOperator::ARROW)
            ));
            continue;
        }

        // Handle postfix operators
        auto postfixOperator = parsePostfixUnaryOperator();
        if (postfixOperator.has_value()) {
            left = std::make_unique<IExpression>(UnaryExpr(
                interval.getInterval(),
                postfixOperator.value(),
                std::move(left)
            ));
        }

        // Handle infix operators
        auto binop = binaryOperatorFromToken(_context.peek()->getType());
        if (!binop.has_value()) {
            break;
        }
        BinaryPrecedence opPrec = binaryPrecedenceValue(binop.value());
        if (opPrec < minBP) {
            break;
        }
        _context.take();

        // Assignments are right-associative (a = b = c  →  a = (b = c)).
        // All other binary operators are left-associative.
        BinaryPrecedence rightMinBP = isAssignmentOperator(binop.value()) ? opPrec : opPrec + 1;
        auto right = parseExpression(rightMinBP);
        if (!right) {
            Throw(ParseError, "Expected expression, found '%s'", _context.peek()->toString().c_str());
        }
        left = std::make_unique<IExpression>(BinaryExpr(
            interval.getInterval(),
            std::move(left),
            std::move(right),
            binop.value()
        ));
    }

    return left;
}

std::unique_ptr<IExpression> Parser::parseExpressionAtom()
{
    if (_context.takeIf(tok::TokenType::PAREN_L)) {
        auto inner = parseExpression(BP_NONE);
        if (!inner)
            Throw(ParseError, "Expected expression after '('");
        _context.require(tok::TokenType::PAREN_R);
        return inner;
    }

    std::unique_ptr<IExpression> e;
    if (    ((e = parseFunctionCallExpr()))
        ||  ((e = parseVarRefExpr()))
        ||  ((e = parseCharLiteralExpr()))
        ||  ((e = parseIntLiteralExpr()))
        ||  ((e = parseFloatLiteralExpr()))
        ||  ((e = parseStringLiteralExpr()))
    ) {
        return e;
    }

    return nullptr;
}

std::unique_ptr<IExpression> Parser::parseFunctionCallExpr()
{
    auto interval = _context.getIntervalReader();
    auto transaction = _context.beginTransaction();

    auto functionName = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!functionName) return nullptr;

    if (!_context.takeIf(tok::TokenType::PAREN_L)) return nullptr;

    std::vector<std::unique_ptr<IExpression>> params;
    std::unique_ptr<IExpression> p;

    bool expectParam = false;

    while (!_context.takeIf(tok::TokenType::PAREN_R)) {
        auto expr = parseExpression(BP_NONE);
        if (!expr) {
            Throw(ParseError, "Unable to parse function parameter");
        }
        params.push_back(std::move(expr));
        expectParam = false;
        if (_context.takeIf(tok::TokenType::COMMA)) {
            expectParam = true;
        }
    }
    if (expectParam) {
        Throw(ParseError, "Expected parameter, found ')'");
    }

    transaction.commit();
    return std::make_unique<IExpression>(
        FunctionCallExpr(
            interval.getInterval(),
            functionName->getLexeme(),
            std::move(params)
        )
    );
}

std::unique_ptr<IExpression> Parser::parseVarRefExpr()
{
    auto interval = _context.getIntervalReader();
    auto identifier = _context.takeIf(tok::TokenType::IDENTIFIER);
    if (!identifier) {
        return nullptr;
    }

    return std::make_unique<IExpression>(
        VarRefExpr(interval.getInterval(), identifier->getLexeme())
    );
}

std::unique_ptr<IExpression> Parser::parseCharLiteralExpr()
{
    auto interval = _context.getIntervalReader();
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

    return std::make_unique<IExpression>(
        CharLiteralExpr(interval.getInterval(), value)
    );
}

std::unique_ptr<IExpression> Parser::parseIntLiteralExpr()
{
    auto interval = _context.getIntervalReader();
    const auto& token = _context.takeIf(tok::TokenType::LIT_INT);
    if (!token) {
        return nullptr;
    }

    uint64_t value = 0;
    size_t parsed = 0;

    const auto& lexeme = token->getLexeme();
    if (lexeme.starts_with("0x") || lexeme.starts_with("0X")) {
        value = std::stoll(lexeme.substr(2), &parsed, 16);
        parsed += 2;
    } else if (lexeme.starts_with("0b") || lexeme.starts_with("0B")) {
        value = std::stoll(lexeme.substr(2), &parsed, 2);
        parsed += 2;
    } else if (lexeme.starts_with("0") && lexeme.length() > 1) {
        value = std::stoll(lexeme.substr(1), &parsed, 8);
        parsed += 1;
    } else {
        value = std::stoi(lexeme, &parsed, 10);
    }

    if (parsed != lexeme.size()) {
        Throw(ParseError, "Unexpected characters in int literal: %s", token->toString().c_str());
    }

    return std::make_unique<IExpression>(
        IntLiteralExpr(interval.getInterval(), value)
    );
}

std::unique_ptr<IExpression> Parser::parseFloatLiteralExpr()
{
    auto interval = _context.getIntervalReader();
    const auto& token = _context.takeIf(tok::TokenType::LIT_FLOAT);
    if (!token) {
        return nullptr;
    }

    const auto& lexeme = token->getLexeme();

    size_t parsed = 0;
    const double value = std::stod(lexeme, &parsed);
    if (parsed < lexeme.size()) {
        if (parsed+1 != lexeme.size() || (lexeme[parsed] != 'f' && lexeme[parsed] != 'F')) {
            Throw(ParseError, "Unexpected characters in float literal: %s", token->toString().c_str());
        }
    }

    return std::make_unique<IExpression>(
        FloatLiteralExpr(interval.getInterval(), value)
    );
}

std::unique_ptr<IExpression> Parser::parseStringLiteralExpr()
{
    auto interval = _context.getIntervalReader();
    const auto& token = _context.takeIf(tok::TokenType::LIT_STRING);
    if (!token) {
        return nullptr;
    }

    const auto& lexeme = token->getLexeme();
    assert(lexeme.size() >= 2);
    std::string value = lexeme.substr(1, lexeme.size() - 2);
    return std::make_unique<IExpression>(
        StringLiteralExpr(interval.getInterval(), value)
    );
}

std::optional<ISizeofTerm> Parser::parseSizeofTerm()
{
    if (_context.peek()->getType() == (tok::TokenType::PAREN_L)) {
        auto transaction = _context.beginTransaction();
        _context.take();

        // Note: we may still receive variable references here, because we don't
        // yet have a way of separating "myVar" from "int".
        if (auto type = parseTypeIdentifier(); type.has_value()) {
            if (_context.takeIf(tok::TokenType::PAREN_R)) {
                transaction.commit();
                return type.value();
            }
        }
    }

    if (auto expr = parseExpression(BP_SIZEOF)) {
        return expr;
    }

    return std::nullopt;
}

std::optional<UnaryOperator> Parser::parsePrefixUnaryOperator()
{
    UnaryOperator oper;
    switch (_context.peek()->getType()) {
        case tok::TokenType::INCREMENT: oper = UnaryOperator::PREINC; break;
        case tok::TokenType::DECREMENT: oper = UnaryOperator::PREDEC; break;
        case tok::TokenType::MINUS: oper = UnaryOperator::MINUS; break;
        case tok::TokenType::BANG: oper = UnaryOperator::NEGATE; break;
        case tok::TokenType::TILDE: oper = UnaryOperator::ONES_COMPL; break;
        case tok::TokenType::STAR: oper = UnaryOperator::DEREF; break;
        case tok::TokenType::AMPERSAND: oper = UnaryOperator::ADDROF; break;
        case tok::TokenType::SIZEOF: oper = UnaryOperator::SIZEOF; break;
        default: return std::nullopt;
    }

    _context.take();
    return oper;
}

std::optional<BinaryOperator> Parser::parseBinaryOperator()
{
    std::optional<BinaryOperator> oper = binaryOperatorFromToken(_context.peek()->getType());

    if (oper.has_value()) {
        _context.take();
        return oper.value();
    }
    return std::nullopt;
}

std::optional<UnaryOperator> Parser::parsePostfixUnaryOperator()
{
    UnaryOperator oper;
    switch (_context.peek()->getType()) {
        case tok::TokenType::INCREMENT: oper = UnaryOperator::POSTINC; break;
        case tok::TokenType::DECREMENT: oper = UnaryOperator::POSTDEC; break;
        default: return std::nullopt;
    }

    _context.take();
    return oper;
}

std::optional<TypeIdentifier> Parser::parseTypeCastOperator()
{
    auto transaction = _context.beginTransaction();

    if (!_context.takeIf(tok::TokenType::PAREN_L)) {
        return std::nullopt;
    }

    auto type = parseTypeIdentifier();
    if (!type.has_value()) {
        return std::nullopt;
    }

    if (!_context.takeIf(tok::TokenType::PAREN_R)) {
        return std::nullopt;
    }
    transaction.commit();
    return type;
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

