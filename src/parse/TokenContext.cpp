#include "TokenContext.h"
#include "../Exception.h"
#include <_strings.h>

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
    : context(context)
    , index(index)
    , contextId(contextId)
{ }

TokenContext::Transaction::~Transaction()
{
    context->endTransaction(*this);
}

void TokenContext::Transaction::commit()
{
    action = TransactionAction::COMMIT;
}

/*
================
TokenContext
================
*/
TokenContext::TokenContext(const std::vector<tok::Token>& tokens)
    : _tokens(tokens)
    , _index(0)
    , _contextId(0)
{
    assert(_tokens.size() > 0 && _tokens.back().getType() == tok::TokenType::END_OF_FILE);
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

const tok::Token* TokenContext::peek() const
{
    return &_tokens[_index];
}

const tok::Token* TokenContext::peekAhead(unsigned int offset) const
{
    if (_index + offset < _tokens.size()) {
        return &_tokens[_index + offset];
    }

    // Return the EOF token
    return &_tokens[_tokens.size() - 1];
}

const tok::Token* TokenContext::take()
{
    if (!atEnd()) {
        return &_tokens[_index++];
    }
    return nullptr;
}

const tok::Token* TokenContext::takeIf(tok::TokenType type)
{
    if (!atEnd() && _tokens[_index].getType() == type) {
        return &_tokens[_index++];
    }
    return nullptr;
}

const tok::Token* TokenContext::require(tok::TokenType type)
{
    auto token = takeIf(type);
    if (!token) {
        Throw(UnexpectedToken, "Unexpected token: %s", peek()->toString().c_str());
    }
    return token;
}

void TokenContext::exhaustSemicolons()
{
    while (!atEnd() && takeIf(tok::TokenType::SEMICOLON)) { }
}

void TokenContext::endTransaction(const Transaction& transaction)
{
    assert(transaction.contextId == _contextId);
    switch (transaction.action) {
        case TransactionAction::REVERT:
            _index = transaction.index;
            break;
        case TransactionAction::COMMIT:
            break;
    }
}

}
