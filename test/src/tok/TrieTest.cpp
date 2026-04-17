#include <gtest/gtest.h>

#include "tok/Trie.h"

using namespace cish::tok;

TEST(TrieTest, TestSingleCharEntries)
{
    TokenTrie trie;
    trie.insert("(", TokenType::PAREN_L);
    trie.insert(")", TokenType::PAREN_R);
    trie.insert("+", TokenType::PLUS);

    TokenTrie::Result exp;

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
    TokenTrie trie;
    trie.insert("semicolon", TokenType::SEMICOLON);
    trie.insert("colon", TokenType::COLON);

    TokenTrie::Result exp;

    exp = { true, 9, TokenType::SEMICOLON };
    ASSERT_EQ(exp, trie.search("semicolons are nice"));

    exp = { true, 5, TokenType::COLON };
    ASSERT_EQ(exp, trie.search("colonoscopy"));
}

TEST(TrieTest, LongestWordIsReturned)
{
    TokenTrie trie;
    trie.insert(">>=", TokenType::RS_ASSIGN);
    trie.insert(">>", TokenType::RSHIFT);
    trie.insert(">", TokenType::ABRACE_R);

    TokenTrie::Result exp;

    exp = { true, 3, TokenType::RS_ASSIGN };
    ASSERT_EQ(exp, trie.search(">>= 3"));

    exp = { true, 2, TokenType::RSHIFT };
    ASSERT_EQ(exp, trie.search(">> mye"));

    exp = { true, 1, TokenType::ABRACE_R };
    ASSERT_EQ(exp, trie.search("> langt"));
}

TEST(TrieTest, SimpleWordSalad)
{
    Trie<bool, const char, 0, 255> trie;
    trie.insert("eple", true);
    trie.insert("eplekake", true);

    ASSERT_FALSE(trie.search("pære er en frukt").success);
    ASSERT_TRUE(trie.search("eple er noe annet").success);
    ASSERT_TRUE(trie.search("eplekake er brunt").success);
    ASSERT_TRUE(trie.search("eplefjes").success); 
}
