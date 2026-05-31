#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <fstream>

#include "lex/Lexer.h"

using namespace cish::lex;

TEST(LexerTest, BasicPrimitives)
{
    const std::string src = "(+-";
    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();

    ASSERT_EQ(4, tokens.size());
    ASSERT_EQ(Token(TokenType::PAREN_L, "(", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::PLUS, "+", 1, 1), tokens[1]);
    ASSERT_EQ(Token(TokenType::MINUS, "-", 1, 2), tokens[2]);
    ASSERT_EQ(Token(TokenType::END_OF_FILE, "", 1, 3), tokens[3]);
}

TEST(LexerTest, SimpleLexerTest)
{
    const std::string src = "int a = 5";
    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();

    ASSERT_EQ(5, tokens.size());

    ASSERT_EQ(Token(TokenType::IDENTIFIER, "int", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::IDENTIFIER, "a", 1, 4), tokens[1]);
    ASSERT_EQ(Token(TokenType::EQUAL, "=", 1, 6), tokens[2]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 1, 8), tokens[3]);
    ASSERT_EQ(Token(TokenType::END_OF_FILE, "", 1, 9), tokens[4]);
}

TEST(LexerTest, StringLiterals)
{
    const std::string src = R"("wtf \"er\" dette??"   )";
    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();

    ASSERT_EQ(2, tokens.size());
    ASSERT_EQ(TokenType::LIT_STRING, tokens[0].getType());
    ASSERT_EQ("\"wtf \\\"er\\\" dette??\"", tokens[0].getLexeme());

    ASSERT_EQ(TokenType::END_OF_FILE, tokens[1].getType());
}

TEST(LexerTest, BlockCommentsAreNotReturned)
{
    const std::string src = "return /* ignore this\nand this\n*/5";
    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();
    ASSERT_EQ(3, tokens.size());
    ASSERT_EQ(Token(TokenType::RETURN, "return", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 3, 2), tokens[1]);

    ASSERT_EQ(Token(TokenType::END_OF_FILE, "", 3, 3), tokens[2]);
}

TEST(LexerTest, LineCommentsAreNotReturned)
{
    const std::string src = "return // ignore_this\n5";
    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();
    ASSERT_EQ(3, tokens.size());
    ASSERT_EQ(Token(TokenType::RETURN, "return", 1, 0), tokens[0]);
    ASSERT_EQ(Token(TokenType::LIT_INT, "5", 2, 0), tokens[1]);

    ASSERT_EQ(Token(TokenType::END_OF_FILE, "", 2, 1), tokens[2]);
}

TEST(LexerTest, VerifyFullTokenization)
{
    const std::string src = R"(
    #include <std/_lib.h>
    int main(const char** argv, int argc)
    {
        /**
         * Everything in this program should be tokenized.
         */
        // yess
        float f = .4f;
        f = 0.5 + 0.013f;
        int n = 0x14;
        n >>= ++n - 1;
        return 0;
    }
    )";

    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();

    std::vector<TokenType> actual;
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(actual), [](auto t) { return t.getType(); });

    std::vector<TokenType> expected = {
        TokenType::INCLUDE_SYS,         // #include <std/_lib.h>
        TokenType::IDENTIFIER,          // int
        TokenType::IDENTIFIER,          // main
        TokenType::PAREN_L,             // (
        TokenType::CONST,               // const
        TokenType::IDENTIFIER,          // char
        TokenType::STAR,                // *
        TokenType::STAR,                // *
        TokenType::IDENTIFIER,          // argv
        TokenType::COMMA,
        TokenType::IDENTIFIER,          // int
        TokenType::IDENTIFIER,          // argc
        TokenType::PAREN_R,             // )
        TokenType::CBRACE_L,            // {
        TokenType::IDENTIFIER,          // float
        TokenType::IDENTIFIER,          // f
        TokenType::EQUAL,               // =
        TokenType::LIT_FLOAT,           // .4f
        TokenType::SEMICOLON,
        TokenType::IDENTIFIER,          // f
        TokenType::EQUAL,               // =
        TokenType::LIT_FLOAT,           // 0.5f
        TokenType::PLUS,                // +
        TokenType::LIT_FLOAT,           // 0.013f
        TokenType::SEMICOLON,
        TokenType::IDENTIFIER,          // int
        TokenType::IDENTIFIER,          // n
        TokenType::EQUAL,               // =
        TokenType::LIT_INT,             // 0x14
        TokenType::SEMICOLON,
        TokenType::IDENTIFIER,          // n
        TokenType::RS_ASSIGN,           // >>=
        TokenType::INCREMENT,           // ++
        TokenType::IDENTIFIER,          // n
        TokenType::MINUS,               // -
        TokenType::LIT_INT,             // 1
        TokenType::SEMICOLON,
        TokenType::RETURN,              // return
        TokenType::LIT_INT,             // 0
        TokenType::SEMICOLON,
        TokenType::CBRACE_R,            // }
        TokenType::END_OF_FILE,
    };

    if (expected != actual) {
        std::cout << std::setw(12) << "EXPECTED" << "|" << std::setw(12) << "ACTUAL" << std::endl;
        for (int i=0; i<std::max(expected.size(), actual.size()); i++) {
            std::ostringstream exp, act;
            if (i < expected.size()) {
                exp << expected[i];
            }
            if (i < actual.size()) {
                act << actual[i];
            }

            std::cout << std::setw(12) << exp.str() << "|" << std::setw(12) << act.str() << std::endl;
        }
    }
    ASSERT_EQ(expected, actual);
}

TEST(LexerTest, UnexpectedTokensThrows)
{
    const std::string source = "#";
    Lexer tokenizer(source);
    ASSERT_THROW(tokenizer.tokenize(), cish::lex::TokenizerError);
}

TEST(LexerTest, RepeatedSemicolonsAreIgnored)
{
    const std::string src = R"(
        ;;;; int 5 ;;;
    )";

    Lexer tokenizer(src);
    const auto tokens = tokenizer.tokenize();

    std::vector<TokenType> actual;
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(actual), [](auto t) { return t.getType(); });

    std::vector expected = {
        TokenType::SEMICOLON,
        TokenType::IDENTIFIER,
        TokenType::LIT_INT,
        TokenType::SEMICOLON,
        TokenType::END_OF_FILE,
    };

    ASSERT_EQ(expected, actual);
}

TEST(LexerTest, VerifyGCCTestSuiteTokenizesCleanly)
{
    // This test may not actually work, and that is fine.
    // Traverse the directories upwards to find the 'gcc_compare/'-directory.
    // Unless we're running from a different hierarchy entirely, we should begin
    // able to find it within 5 parent dirs.
    const int maxDirs = 5;
    int steps = 0;

    std::string dir = "./";

    while (!std::filesystem::exists(dir + "gcc_compare/")) {
        dir += "../";
        steps++;
        if (steps >= maxDirs) {
            printf("Max directory attempts reached (%d), aborting test\n", maxDirs);
            return;
        }
    }

    for (const auto& file: std::filesystem::directory_iterator(dir + "gcc_compare/")) {
        if (file.path().extension() == ".c") {
            printf("Testing file '%s'\n", file.path().filename().c_str());
            std::ifstream ifs(file.path());
            ifs.seekg(0, std::ios::end);
            const size_t size = ifs.tellg();
            ifs.seekg(0);
            std::string buffer(size, '\0');
            ifs.read(&buffer[0], size);

            // We have no idea what the file contains, we only know that
            // it shouldn't throw an error to tokenize it.
            Lexer s(buffer);
            std::vector<Token> tokens;
            ASSERT_NO_THROW(tokens = s.tokenize());
            ASSERT_NE(0, tokens.size());
        }
    }
}

