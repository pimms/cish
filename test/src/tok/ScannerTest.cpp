#include <gtest/gtest.h>
#include <string>

#include "tok/Scanner.h"

using namespace cish::tok;

TEST(ScannerTest, SimpleTokenizerTest)
{
    const std::string src = "int a = 5";
    Scanner scanner(src);
    const std::vector<Token> tokens = scanner.tokenize();

    ASSERT_EQ(5, tokens.size());

    ASSERT_EQ(Token(TokenType::IDENTIFIER, "int", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::IDENTIFIER, "a", 1, 4), tokens[1]);
    ASSERT_EQ(Token(TokenType::EQUAL, "=", 1, 6), tokens[2]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 1, 8), tokens[3]);
}
