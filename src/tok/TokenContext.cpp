#include "TokenContext.h"
#include "../Exception.h"
#include <_strings.h>

namespace cish::tok
{

DECLARE_EXCEPTION(TokenContextInvalidTransaction);

TokenContext::TokenContext(std::vector<Token>& tokens)
    : _tokens(tokens)
    , _backIndex(-1)
    , _index(0)
{ }

void TokenContext::reset()
{
    _tokens = {};
    _backIndex = -1;
    _index = 0;
}

void TokenContext::begin()
{
    if (_backIndex != -1) {
        Throw(TokenContextInvalidTransaction, "A transaction is already active");
    }
    _backIndex = _index;
}

void TokenContext::revert()
{
    if (_backIndex == -1) {
        Throw(TokenContextInvalidTransaction, "No transaction active");
    }

    _index = _backIndex;
    _backIndex = -1;
}

void TokenContext::commit()
{
    _backIndex = -1;
}

const Token* TokenContext::peek() const
{
    if (_index < _tokens.size()) {
        return &_tokens[_index];
    }
    return nullptr;
}

const Token* TokenContext::take()
{
    if (_index < _tokens.size()) {
        return &_tokens[_index++];
    }
    return nullptr;
}

}
