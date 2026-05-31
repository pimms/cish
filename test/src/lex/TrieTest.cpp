#include <gtest/gtest.h>
#include <optional>

#include "lex/Trie.h"

using namespace cish::lex;

TEST(TrieTest, TestSingleCharEntries)
{
    TokenTrie trie;
    trie.insert("(", TokenType::PAREN_L);
    trie.insert(")", TokenType::PAREN_R);
    trie.insert("+", TokenType::PLUS);

    TokenTrie::Result exp;

    exp = { TokenType::PAREN_L, 1 };
    ASSERT_EQ(exp, trie.search("(aaerhajer"));

    exp = { TokenType::PAREN_R, 1 };
    ASSERT_EQ(exp, trie.search(")"));

    exp = { TokenType::PLUS, 1 };
    ASSERT_EQ(exp, trie.search("+"));

    ASSERT_EQ(std::nullopt, trie.search(""));
    ASSERT_EQ(std::nullopt, trie.search("-"));
}

TEST(TrieTest, TestWords)
{
    TokenTrie trie;
    trie.insert("semicolon", TokenType::SEMICOLON);
    trie.insert("colon", TokenType::COLON);

    TokenTrie::Result exp;

    exp = { TokenType::SEMICOLON, 9 };
    ASSERT_EQ(exp, trie.search("semicolons are nice"));

    exp = { TokenType::COLON, 5 };
    ASSERT_EQ(exp, trie.search("colonoscopy"));
}

TEST(TrieTest, LongestWordIsReturned)
{
    TokenTrie trie;
    trie.insert(">>=", TokenType::RS_ASSIGN);
    trie.insert(">>", TokenType::RSHIFT);
    trie.insert(">", TokenType::ABRACE_R);

    TokenTrie::Result exp;

    exp = { TokenType::RS_ASSIGN, 3 };
    ASSERT_EQ(exp, trie.search(">>= 3"));

    exp = { TokenType::RSHIFT, 2 };
    ASSERT_EQ(exp, trie.search(">> mye"));

    exp = { TokenType::ABRACE_R, 1 };
    ASSERT_EQ(exp, trie.search("> langt"));
}

TEST(TrieTest, SimpleWordSalad)
{
    Trie<bool, const char, 0, 255> trie;
    trie.insert("eple", true);
    trie.insert("eplekake", true);

    ASSERT_EQ(std::nullopt, trie.search("pære er en frukt"));
    ASSERT_NE(std::nullopt, trie.search("eple er noe annet"));
    ASSERT_NE(std::nullopt, trie.search("eplekake er brunt"));
    ASSERT_NE(std::nullopt, trie.search("eplefjes")); 
}
