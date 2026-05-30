#pragma once

#include <vector>
#include "../tok/Token.h"
#include "CodeMarker.h"

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
        TokenContext *_context;
        const int _index;
        const int _contextId;
    };

    class CodeIntervalReader {
    public:
        CodeIntervalReader(const CodeIntervalReader&) = delete;
        CodeIntervalReader(CodeIntervalReader&&) = delete;
        CodeIntervalReader& operator=(const CodeIntervalReader&) = delete;
        CodeIntervalReader& operator=(CodeIntervalReader&&) = delete;
        CodeInterval getInterval() const;
    protected:
        friend class TokenContext;
        explicit CodeIntervalReader(TokenContext* context);
    private:
        TokenContext *_context;
        CodeMarker _begin;
    };

    explicit TokenContext(const std::vector<tok::Token>& tokens);
    TokenContext() = delete;
    TokenContext(const TokenContext&) = delete;
    TokenContext& operator=(const TokenContext&) = delete;
    TokenContext& operator=(TokenContext&&) = delete;
    ~TokenContext() = default;

    void reset();
    Transaction beginTransaction();
    CodeIntervalReader getIntervalReader();

    bool atEnd() const { return _tokens[_index].getType() == tok::TokenType::END_OF_FILE; }
    const tok::Token* peek() const;
    const tok::Token* peekRelative(int offset) const;
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
