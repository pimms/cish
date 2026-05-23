#include <gtest/gtest.h>
#include <tok/TokenType.h>
#include <variant>
#include <filesystem>
#include <fstream>

#include "parse/Parser.h"
#include "tok/Tokenizer.h"

using namespace cish::parse;
using namespace cish::tok;

std::unique_ptr<ParseTree> parse(const std::string& source)
{
    Tokenizer tokenizer(source);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens);
    return parser.parse();
}

TEST(ParserTest, ParseSystemIncludes)
{
    auto tree = parse(R"(
        #include <stdio.h>
        #include <cstdio>
        #include <net/inet.h>
        #include <wine_me_dine_me/69_me.h>
    )");

    ASSERT_EQ(4, tree->rootItems.size());

    for (int i=0; i<4; i++) {
        ASSERT_TRUE(std::holds_alternative<SystemInclude>(tree->rootItems[i]));
    }

    ASSERT_EQ("stdio.h", std::get<SystemInclude>(tree->rootItems[0]).moduleName);
    ASSERT_EQ("cstdio", std::get<SystemInclude>(tree->rootItems[1]).moduleName);
    ASSERT_EQ("net/inet.h", std::get<SystemInclude>(tree->rootItems[2]).moduleName);
    ASSERT_EQ("wine_me_dine_me/69_me.h", std::get<SystemInclude>(tree->rootItems[3]).moduleName);
}

TEST(ParserTest, ParseStructDeclaration)
{
    auto tree = parse(R"(
        ;;;struct Foo
        {
            ;;int n;;;;;;;;
        // TISS
        ;;;
            const int x;;;; /* promp */
            ;; const struct Foo **arr;
        };;;;
    )");

    ASSERT_EQ(1, tree->rootItems.size());

    ASSERT_TRUE(std::holds_alternative<StructDeclaration>(tree->rootItems[0]));
    auto decl = std::get<StructDeclaration>(tree->rootItems[0]);
    ASSERT_EQ("Foo", decl.name);
    ASSERT_EQ(3, decl.fields.size());

    auto t = TypeIdentifier { .isConst = false, .isStruct = false, .type = "int", .pointerLevel = 0};

    ASSERT_EQ("n", decl.fields[0].name);
    ASSERT_EQ(TypeIdentifier(false, false, "int", 0), decl.fields[0].type);

    ASSERT_EQ("x", decl.fields[1].name);
    ASSERT_EQ(TypeIdentifier(true, false, "int", 0), decl.fields[1].type);

    ASSERT_EQ("arr", decl.fields[2].name);
    ASSERT_EQ(true, decl.fields[2].type.isConst);
    ASSERT_EQ(true, decl.fields[2].type.isStruct);
    ASSERT_EQ("Foo", decl.fields[2].type.type);
    ASSERT_EQ(2, decl.fields[2].type.pointerLevel);
}

TEST(ParserTest, ParseFunctionDeclaration)
{
    auto tree = parse(R"(
        void foo(const struct bar*);
        const int main (int argc, const
                        char*
                        * /* jesus christ*/
                        argv)  ;;;
    )");

    ASSERT_EQ(2, tree->rootItems.size());

    // verify foo
    ASSERT_TRUE(std::holds_alternative<FunctionDeclaration>(tree->rootItems[0]));
    FunctionDeclaration fooDecl = std::get<FunctionDeclaration>(tree->rootItems[0]);
    auto fooExp = FunctionDeclaration {
        .returnType = TypeIdentifier { .isConst = false, .isStruct = false, .type = "void", .pointerLevel = 0 },
        .name = "foo",
        .params = {
            FunctionParameter {
                .type = TypeIdentifier { .isConst=true, .isStruct=true, .type="bar", .pointerLevel=1 },
                .name = std::nullopt
            }
        }
    };
    ASSERT_EQ(fooExp, fooDecl);

    // Verify main
    ASSERT_TRUE(std::holds_alternative<FunctionDeclaration>(tree->rootItems[1]));
    const auto& mainDecl = std::get<FunctionDeclaration>(tree->rootItems[1]);
    auto mainExp = FunctionDeclaration {
        .returnType = TypeIdentifier { .isConst = true, .isStruct = false, .type = "int", .pointerLevel = 0 },
        .name = "main",
        .params = {
            FunctionParameter {
                .type = TypeIdentifier { .isConst=false, .isStruct=false, .type="int", .pointerLevel=0 },
                .name = "argc"
            },
            FunctionParameter {
                .type = TypeIdentifier { .isConst=true, .isStruct=false, .type="char", .pointerLevel=2 },
                .name = "argv"
            },
        }
    };
    ASSERT_EQ(mainExp, mainDecl);
}

TEST(ParserTest, ParseGlobalVariables)
{
    auto tree = parse(R"(
        const int random_ass_var;
    )");

    ASSERT_EQ(1, tree->rootItems.size());
    ASSERT_TRUE(std::holds_alternative<VariableDeclarationStatement>(tree->rootItems[0]));

    const auto& decl = std::get<VariableDeclarationStatement>(tree->rootItems[0]);
    auto expected = VariableDeclarationStatement {
        .type = TypeIdentifier {
            .isConst = true,
            .isStruct = false,
            .type = "int",
            .pointerLevel = 0
        },
        .varName = "random_ass_var",
        .expression = nullptr
    };
    ASSERT_EQ(expected, decl);
}

TEST(ParserTest, ParseGccComparisonSuite)
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
            Tokenizer s(buffer);
            std::vector<Token> tokens;
            ASSERT_NO_THROW(tokens = s.tokenize());
            ASSERT_NE(0, tokens.size());

            Parser parser(tokens);
            ASSERT_NO_THROW(parser.parse());
        }
    }
}
