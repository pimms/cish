#include "TokenContext.h"
#include "../Exception.h"

namespace cish::parse
{

DECLARE_EXCEPTION(TokenContextInvalidTransaction);
DECLARE_EXCEPTION(UnexpectedToken);

/*
================
TokenContext::Transaction
================
*/
TokenContext::Transaction::Transaction(TokenContext* context, int index, int contextId)
    : _context(context)
    , _index(index)
    , _contextId(contextId)
{ }

TokenContext::Transaction::~Transaction()
{
    _context->endTransaction(*this);
}

void TokenContext::Transaction::commit()
{
    action = TransactionAction::COMMIT;
}

/*
================
TokenContext::CodeIntervalReporter
================
*/
TokenContext::CodeIntervalReader::CodeIntervalReader(TokenContext *context)
    : _context(context)
{
    const lex::Token* token = context->peek();
    _begin = CodeMarker {
        .line = token->getLine(),
        .col = token->getCol(),
        .charOffset = token->getCharOffset()
    };
}

CodeInterval TokenContext::CodeIntervalReader::getInterval() const
{
    const lex::Token *token{};
    if (_context->peek()->getCharOffset() == _begin.charOffset) {
        // Only one token was consumed. The end is the end of this token.
        token = _context->peek();
    } else {
        // Multiple tokens consumed. Consider the end of the last token the end of the interval.
        token = _context->peekRelative(-1);
    }

    CodeMarker end = CodeMarker {
        .line = token->getLine(),
        .col = token->getCol(),
        .charOffset = token->getCharOffset() + (int)token->getLexeme().length()
    };
    return CodeInterval(_begin, end);
}

/*
================
TokenContext
================
*/
TokenContext::TokenContext(const std::vector<lex::Token>& tokens)
    : _tokens(tokens)
    , _index(0)
    , _contextId(0)
{
    assert(_tokens.size() > 0 && _tokens.back().getType() == lex::TokenType::END_OF_FILE);
}

void TokenContext::reset()
{
    _index = 0;
    _contextId++;
}

TokenContext::Transaction TokenContext::beginTransaction()
{
    return Transaction(this, _index, _contextId);
}

TokenContext::CodeIntervalReader TokenContext::getIntervalReader()
{
    return CodeIntervalReader(this);
}

const lex::Token* TokenContext::peek() const
{
    return &_tokens[_index];
}

const lex::Token* TokenContext::peekRelative(int offset) const
{
    assert(_index + offset >= 0);

    if (_index + offset < _tokens.size()) {
        return &_tokens[_index + offset];
    }

    // Return the EOF token
    return &_tokens[_tokens.size() - 1];
}

const lex::Token* TokenContext::take()
{
    if (!atEnd()) {
        return &_tokens[_index++];
    }
    return nullptr;
}

const lex::Token* TokenContext::takeIf(lex::TokenType type)
{
    if (!atEnd() && _tokens[_index].getType() == type) {
        return &_tokens[_index++];
    }
    return nullptr;
}

const lex::Token* TokenContext::require(lex::TokenType type)
{
    auto token = takeIf(type);
    if (!token) {
        Throw(UnexpectedToken, "Unexpected token: %s", peek()->toString().c_str());
    }
    return token;
}

void TokenContext::exhaustSemicolons()
{
    while (!atEnd() && takeIf(lex::TokenType::SEMICOLON)) { }
}

void TokenContext::endTransaction(const Transaction& transaction)
{
    assert(transaction._contextId == _contextId);
    switch (transaction.action) {
        case TransactionAction::REVERT:
            _index = transaction._index;
            break;
        case TransactionAction::COMMIT:
            break;
    }
}

}
