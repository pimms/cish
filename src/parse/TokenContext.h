#pragma once

#include <vector>
#include "../lex/Token.h"
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

    explicit TokenContext(const std::vector<lex::Token>& tokens);
    TokenContext() = delete;
    TokenContext(const TokenContext&) = delete;
    TokenContext& operator=(const TokenContext&) = delete;
    TokenContext& operator=(TokenContext&&) = delete;
    ~TokenContext() = default;

    void reset();
    Transaction beginTransaction();
    CodeIntervalReader getIntervalReader();

    bool atEnd() const { return _tokens[_index].getType() == lex::TokenType::END_OF_FILE; }
    const lex::Token* peek() const;
    const lex::Token* peekRelative(int offset) const;
    const lex::Token* take();
    const lex::Token* takeIf(lex::TokenType type);
    const lex::Token* require(lex::TokenType type);

    void exhaustSemicolons();

protected:
    friend class Transaction;
    void endTransaction(const Transaction& transaction);

private:
    std::vector<lex::Token> _tokens;
    int _index;
    int _contextId;
};

}
