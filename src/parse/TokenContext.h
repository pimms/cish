#pragma once

#include <vector>
#include "../tok/Token.h"

namespace cish::parse
{

class TokenContext
{
    enum class TransactionAction {
        COMMIT,
        REVERT
    };
public:
    class Transaction {
    public:
        ~Transaction();
        void commit();

        Transaction(const Transaction&) = delete;
        Transaction(Transaction&&) = delete;
        Transaction& operator=(const Transaction&) = delete;
        Transaction& operator=(Transaction&&) = delete;
    protected:
        friend class TokenContext;
        explicit Transaction(TokenContext* context, int index, int contextId);
    private:
        TransactionAction action { TransactionAction::REVERT };
        TokenContext *context;
        const int index;
        const int contextId;
    };

    explicit TokenContext(const std::vector<tok::Token>& tokens);
    TokenContext() = delete;
    TokenContext(const TokenContext&) = delete;
    TokenContext& operator=(const TokenContext&) = delete;
    TokenContext& operator=(TokenContext&&) = delete;
    ~TokenContext() = default;

    void reset();
    Transaction beginTransaction();

    bool atEnd() const { return _tokens[_index].getType() == tok::TokenType::END_OF_FILE; }
    const tok::Token* peek() const;
    const tok::Token* peekAhead(unsigned int offset) const;
    const tok::Token* take();
    const tok::Token* takeIf(tok::TokenType type);
    const tok::Token* require(tok::TokenType type);

    void exhaustSemicolons();

protected:
    friend class Transaction;
    void endTransaction(const Transaction& transaction);

private:
    std::vector<tok::Token> _tokens;
    int _index;
    int _contextId;
};

}
