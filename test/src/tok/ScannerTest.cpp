#include <gtest/gtest.h>
#include <string>

#include "tok/Scanner.h"

using namespace cish::tok;

TEST(ScannerTest, BasicPrimitives)
{
    const std::string src = "(+-";
    Scanner scanner(src);
    const auto tokens = scanner.tokenize();

    ASSERT_EQ(3, tokens.size());
    ASSERT_EQ(Token(TokenType::PAREN_L, "(", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::PLUS, "+", 1, 1), tokens[1]);
    ASSERT_EQ(Token(TokenType::MINUS, "-", 1, 2), tokens[2]);
}

TEST(ScannerTest, SimpleTokenizerTest)
{
    const std::string src = "int a = 5";
    Scanner scanner(src);
    const auto tokens = scanner.tokenize();

    ASSERT_EQ(4, tokens.size());

    ASSERT_EQ(Token(TokenType::IDENTIFIER, "int", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::IDENTIFIER, "a", 1, 4), tokens[1]);
    ASSERT_EQ(Token(TokenType::EQUAL, "=", 1, 6), tokens[2]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 1, 8), tokens[3]);
}

TEST(ScannerTest, StringLiterals)
{
    const std::string src = R"("wtf \"er\" dette??"   )";
    Scanner scanner(src);
    const auto tokens = scanner.tokenize();

    ASSERT_EQ(1, tokens.size());
    ASSERT_EQ(TokenType::LIT_STRING, tokens[0].getType());
    ASSERT_EQ("\"wtf \\\"er\\\" dette??\"", tokens[0].getLexeme());
}

TEST(ScannerTest, BlockCommentsAreNotReturned)
{
    const std::string src = "return /* ignore this\nand this\n*/5";
    Scanner scanner(src);
    const auto tokens = scanner.tokenize();
    ASSERT_EQ(2, tokens.size());
    ASSERT_EQ(Token(TokenType::RETURN, "return", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 3, 2), tokens[1]);
}

TEST(ScannerTest, LineCommentsAreNotReturned)
{
    const std::string src = "return // ignore_this\n5";
    Scanner scanner(src);
    const auto tokens = scanner.tokenize();
    ASSERT_EQ(2, tokens.size());
    ASSERT_EQ(Token(TokenType::RETURN, "return", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 2, 0), tokens[1]);
}
