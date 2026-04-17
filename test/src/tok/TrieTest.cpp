#include <gtest/gtest.h>

#include "tok/Trie.h"

using namespace cish::tok;

TEST(TrieTest, TestSingleCharEntries)
{
    Trie trie;
    trie.insert("(", TokenType::PAREN_L);
    trie.insert(")", TokenType::PAREN_R);
    trie.insert("+", TokenType::PLUS);

    Trie::Result exp;

    exp = { true, 1, TokenType::PAREN_L };
    ASSERT_EQ(exp, trie.search("(aaerhajer"));

    exp = { true, 1, TokenType::PAREN_R };
    ASSERT_EQ(exp, trie.search(")"));

    exp = { true, 1, TokenType::PLUS };
    ASSERT_EQ(exp, trie.search("+"));

    exp = { false, 0 };
    ASSERT_EQ(exp, trie.search(""));

    exp = { false, 0 };
    ASSERT_EQ(exp, trie.search("-"));
}

TEST(TrieTest, TestWords)
{
    Trie trie;
    trie.insert("semicolon", TokenType::SEMICOLON);
    trie.insert("colon", TokenType::COLON);

    Trie::Result exp;

    exp = { true, 9, TokenType::SEMICOLON };
    ASSERT_EQ(exp, trie.search("semicolons are nice"));

    exp = { true, 5, TokenType::COLON };
    ASSERT_EQ(exp, trie.search("colonoscopy"));
}

TEST(TrieTest, LongestWordIsReturned)
{
    Trie trie;
    trie.insert(">>=", TokenType::RS_ASSIGN);
    trie.insert(">>", TokenType::RSHIFT);
    trie.insert(">", TokenType::ABRACE_R);

    Trie::Result exp;

    exp = { true, 3, TokenType::RS_ASSIGN };
    ASSERT_EQ(exp, trie.search(">>= 3"));

    exp = { true, 2, TokenType::RSHIFT };
    ASSERT_EQ(exp, trie.search(">> mye"));

    exp = { true, 1, TokenType::ABRACE_R };
    ASSERT_EQ(exp, trie.search("> langt"));
}
