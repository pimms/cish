#include <gtest/gtest.h>
#include <tok/TokenType.h>
#include <variant>

#include "parse/Parser.h"
#include "tok/Tokenizer.h"

using namespace cish::parse;
using namespace cish::tok;

ParseTree parse(const std::string& source)
{
    Tokenizer tokenizer(source);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens);
    return parser.parse();
}

TEST(ParserTest, ParseSystemIncludes)
{
    ParseTree tree = parse(R"(
        #include <stdio.h>
        #include <cstdio>
        #include <net/inet.h>
        #include <wine_me_dine_me/69_me.h>
    )");

    ASSERT_EQ(4, tree.rootItems.size());

    for (int i=0; i<4; i++) {
        ASSERT_TRUE(std::holds_alternative<SystemInclude>(tree.rootItems[i]));
    }

    ASSERT_EQ("stdio.h", std::get<SystemInclude>(tree.rootItems[0]).moduleName);
    ASSERT_EQ("cstdio", std::get<SystemInclude>(tree.rootItems[1]).moduleName);
    ASSERT_EQ("net/inet.h", std::get<SystemInclude>(tree.rootItems[2]).moduleName);
    ASSERT_EQ("wine_me_dine_me/69_me.h", std::get<SystemInclude>(tree.rootItems[3]).moduleName);
}

TEST(ParserTest, ParseStructDeclaration)
{
    ParseTree tree = parse(R"(
        struct Foo
        {
            int n;;;;;;;;
        // TISS
        ;;;
            const int x;;;; /* promp */
            ;; const struct Foo **arr;
        };;;;
    )");

    ASSERT_EQ(1, tree.rootItems.size());

    ASSERT_TRUE(std::holds_alternative<StructDeclaration>(tree.rootItems[0]));
    auto decl = std::get<StructDeclaration>(tree.rootItems[0]);
    ASSERT_EQ("Foo", decl.name.name);
    ASSERT_EQ(3, decl.fields.size());

    auto t = TypeIdentifier { .isConst = false, .isStruct = false, .type = "int", .pointerLevel = 0};

    ASSERT_EQ("n", decl.fields[0].name.name);
    ASSERT_EQ(TypeIdentifier(false, false, "int", 0), decl.fields[0].type);

    ASSERT_EQ("x", decl.fields[1].name.name);
    ASSERT_EQ(TypeIdentifier(true, false, "int", 0), decl.fields[1].type);

    ASSERT_EQ("arr", decl.fields[2].name.name);
    ASSERT_EQ(true, decl.fields[2].type.isConst);
    ASSERT_EQ(true, decl.fields[2].type.isStruct);
    ASSERT_EQ("Foo", decl.fields[2].type.type);
    ASSERT_EQ(2, decl.fields[2].type.pointerLevel);
}
