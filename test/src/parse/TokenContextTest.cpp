#include <gtest/gtest.h>

#include "parse/TokenContext.h"
#include "lex/Token.h"
#include "lex/TokenType.h"

using namespace cish::parse;
using namespace cish::lex;

TEST(TokenContextTest, TestBasicFunctionality)
{
    std::vector tokens = {
        Token(TokenType::ABRACE_L, "", 0, 0, 0),
        Token(TokenType::ABRACE_R, "", 0, 0, 0),
        Token(TokenType::PAREN_L, "", 0, 0, 0),
        Token(TokenType::PAREN_R, "", 0, 0, 0),
        Token(TokenType::PLUS, "", 0, 0, 0),
        Token(TokenType::STAR, "", 0, 0, 0),
        Token(TokenType::END_OF_FILE, "", 0, 0, 0),
    };

    TokenContext context(tokens);
    ASSERT_FALSE(context.atEnd());
    ASSERT_EQ(TokenType::ABRACE_L, context.peek()->getType());
    ASSERT_EQ(TokenType::ABRACE_L, context.take()->getType());
    ASSERT_EQ(TokenType::ABRACE_R, context.peek()->getType());
    ASSERT_NE(nullptr, context.takeIf(TokenType::ABRACE_R));
    ASSERT_EQ(nullptr, context.takeIf(TokenType::ABRACE_R));
    ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
    ASSERT_EQ(TokenType::PAREN_R, context.take()->getType());
    ASSERT_EQ(TokenType::PLUS, context.take()->getType());
    ASSERT_EQ(TokenType::STAR, context.take()->getType());

    ASSERT_TRUE(context.atEnd());
    ASSERT_EQ(nullptr, context.take());
    ASSERT_EQ(TokenType::END_OF_FILE, context.peek()->getType());
}

TEST(TokenContextTest, TestTransactionRevert)
{
    std::vector tokens = {
        Token(TokenType::ABRACE_L, "", 0, 0, 0),
        Token(TokenType::ABRACE_R, "", 0, 0, 0),
        Token(TokenType::PAREN_L, "", 0, 0, 0),
        Token(TokenType::PAREN_R, "", 0, 0, 0),
        Token(TokenType::END_OF_FILE, "", 0, 0, 0),
    };

    TokenContext context(tokens);

    ASSERT_EQ(TokenType::ABRACE_L, context.take()->getType());

    {
        auto transaction = context.beginTransaction();
        ASSERT_EQ(TokenType::ABRACE_R, context.take()->getType());
        ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
        ASSERT_EQ(TokenType::PAREN_R, context.take()->getType());
    }

    ASSERT_EQ(TokenType::ABRACE_R, context.take()->getType());
    ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
    ASSERT_EQ(TokenType::PAREN_R, context.take()->getType());
}

TEST(TokenContextTest, TestHoldingItWrong)
{
    std::vector tokens = {
        Token(TokenType::ABRACE_L, "", 0, 0, 0),
        Token(TokenType::ABRACE_R, "", 0, 0, 0),
        Token(TokenType::PAREN_L, "", 0, 0, 0),
        Token(TokenType::PAREN_R, "", 0, 0, 0),
        Token(TokenType::END_OF_FILE, "", 0, 0, 0),
    };

    TokenContext context(tokens);

    ASSERT_EQ(TokenType::ABRACE_L, context.take()->getType());

    // Here we're holding it quite wrong. The outer transaction is not committed,
    // while the inner one is. When the outer transaction goes out of scope, it will
    // revert the state back to the initial state at its creation.
    {
        auto tWillRevert = context.beginTransaction();
        ASSERT_EQ(TokenType::ABRACE_R, context.take()->getType());
        {
            auto tWillCommit = context.beginTransaction();
            ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
            tWillCommit.commit();
        }
        ASSERT_EQ(TokenType::PAREN_R, context.peek()->getType());
    }

    ASSERT_EQ(TokenType::ABRACE_R, context.peek()->getType());
}

TEST(TokenContextTest, NestedTransactions)
{
    std::vector tokens = {
        Token(TokenType::ABRACE_L, "", 0, 0, 0),
        Token(TokenType::ABRACE_R, "", 0, 0, 0),
        Token(TokenType::PAREN_L, "", 0, 0, 0),
        Token(TokenType::PAREN_R, "", 0, 0, 0),
        Token(TokenType::END_OF_FILE, "", 0, 0, 0),
    };

    TokenContext context(tokens);

    {
        auto t1 = context.beginTransaction();
        ASSERT_EQ(TokenType::ABRACE_L, context.take()->getType());
        {
            auto t2 = context.beginTransaction();
            ASSERT_EQ(TokenType::ABRACE_R, context.take()->getType());
            t2.commit();
        }
        {
            auto t3 = context.beginTransaction();
            ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
        }
        {
            auto t4 = context.beginTransaction();
            ASSERT_EQ(TokenType::PAREN_L, context.take()->getType());
            {
                auto t5 = context.beginTransaction();
                ASSERT_EQ(TokenType::PAREN_R, context.take()->getType());
                t5.commit();
            }
            t4.commit();
        }
        t1.commit();
    }

    ASSERT_TRUE(context.atEnd());
}
