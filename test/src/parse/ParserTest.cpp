#include <gtest/gtest.h>
#include <lex/TokenType.h>
#include <variant>
#include <filesystem>
#include <fstream>

#include "lex/Lexer.h"
#include "parse/Parser.h"

using namespace cish::parse;
using namespace cish::lex;

std::unique_ptr<ParseTree> parse(const std::string& source)
{
    Lexer tokenizer(source);
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

    auto t = TypeIdentifier { .isConst = false, .isStruct = false, .baseType=  BuiltInType::INT, .pointerLevel = 0};

    ASSERT_EQ("n", decl.fields[0].name);
    ASSERT_EQ(TypeIdentifier(false, false, BuiltInType::INT, 0), decl.fields[0].type);

    ASSERT_EQ("x", decl.fields[1].name);
    ASSERT_EQ(TypeIdentifier(true, false, BuiltInType::INT, 0), decl.fields[1].type);

    ASSERT_EQ("arr", decl.fields[2].name);
    ASSERT_EQ(true, decl.fields[2].type.isConst);
    ASSERT_EQ(true, decl.fields[2].type.isStruct);
    ASSERT_EQ(BaseType("Foo"), decl.fields[2].type.baseType);
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
        .interval = CodeInterval {.start={ 2,8,9},.end={2,34,36}},
        .returnType = TypeIdentifier { .isConst = false, .isStruct = false, .baseType = BuiltInType::VOID, .pointerLevel = 0 },
        .name = "foo",
        .params = {
            FunctionParameter {
                .interval = {.start={2,17,18},.end={2,33,35}},
                .type = TypeIdentifier { .isConst=true, .isStruct=true, .baseType ="bar", .pointerLevel=1 },
                .name = std::nullopt
            }
        }
    };
    ASSERT_EQ(fooExp, fooDecl);

    // Verify main
    ASSERT_TRUE(std::holds_alternative<FunctionDeclaration>(tree->rootItems[1]));
    const auto& mainDecl = std::get<FunctionDeclaration>(tree->rootItems[1]);
    auto mainExp = FunctionDeclaration {
        .interval = {.start={3,8,46},.end={6,28,181}},
        .returnType = TypeIdentifier { .isConst = true, .isStruct = false, .baseType = BuiltInType::INT, .pointerLevel = 0 },
        .name = "main",
        .params = {
            FunctionParameter {
                .interval = {.start={3,24,62},.end={3,28,70}},
                .type = TypeIdentifier { .isConst=false, .isStruct=false, .baseType = BuiltInType::INT, .pointerLevel=0 },
                .name = "argc"
            },
            FunctionParameter {
                .interval = {.start={3,34,72},.end={6,24,180}},
                .type = TypeIdentifier { .isConst=true, .isStruct=false, .baseType = BuiltInType::CHAR, .pointerLevel=2 },
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
        .interval = {.start={2,8,9},.end={2,32,34}},
        .type = TypeIdentifier {
            .isConst = true,
            .isStruct = false,
            .baseType = BuiltInType::INT,
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
            Lexer s(buffer);
            std::vector<Token> tokens;
            ASSERT_NO_THROW(tokens = s.tokenize());
            ASSERT_NE(0, tokens.size());

            Parser parser(tokens);
            ASSERT_NO_THROW(parser.parse());
        }
    }
}

// --- Precedence and associativity tests ---

static const BinaryExpr* getBinary(const IExpression& e) {
    return std::get_if<BinaryExpr>(&e);
}

static const VarRefExpr* getVar(const IExpression& e) {
    return std::get_if<VarRefExpr>(&e);
}

static const IntLiteralExpr* getInt(const IExpression& e) {
    return std::get_if<IntLiteralExpr>(&e);
}

TEST(ParserTest, AssignmentRightAssociativity) {
    // a = b = c = 1  must parse as  a = (b = (c = 1))
    auto tree = parse(R"(
        int dummy = a = b = c = 1;
    )");
    ASSERT_EQ(1, tree->rootItems.size());
    auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
    ASSERT_NE(nullptr, varDecl);
    ASSERT_NE(nullptr, varDecl->expression);

    // Top level of initializer: a = (b = (c = 1))
    const BinaryExpr* top = getBinary(*varDecl->expression);
    ASSERT_NE(nullptr, top);
    ASSERT_EQ(BinaryOperator::ASSIGN, top->oper);

    const VarRefExpr* leftA = getVar(*top->left);
    ASSERT_NE(nullptr, leftA);
    EXPECT_EQ("a", leftA->identifier);

    // Right of top must itself be an assignment
    const BinaryExpr* mid = getBinary(*top->right);
    ASSERT_NE(nullptr, mid);
    ASSERT_EQ(BinaryOperator::ASSIGN, mid->oper);

    const VarRefExpr* leftB = getVar(*mid->left);
    ASSERT_NE(nullptr, leftB);
    EXPECT_EQ("b", leftB->identifier);

    // Right of mid must be the innermost assignment
    const BinaryExpr* inner = getBinary(*mid->right);
    ASSERT_NE(nullptr, inner);
    ASSERT_EQ(BinaryOperator::ASSIGN, inner->oper);

    const VarRefExpr* leftC = getVar(*inner->left);
    ASSERT_NE(nullptr, leftC);
    EXPECT_EQ("c", leftC->identifier);

    const IntLiteralExpr* one = getInt(*inner->right);
    ASSERT_NE(nullptr, one);
    EXPECT_EQ(1, one->value);
}

TEST(ParserTest, MixedPrecedenceWithAssignment) {
    // x = y + z * 2 + 3   should be  x = ((y + (z * 2)) + 3)
    auto tree = parse(R"(
        int dummy = x = y + z * 2 + 3;
    )");
    ASSERT_EQ(1, tree->rootItems.size());
    auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
    ASSERT_NE(nullptr, varDecl);
    ASSERT_NE(nullptr, varDecl->expression);

    // Outermost: x = (the add chain)
    const BinaryExpr* assign = getBinary(*varDecl->expression);
    ASSERT_NE(nullptr, assign);
    ASSERT_EQ(BinaryOperator::ASSIGN, assign->oper);
    const VarRefExpr* x = getVar(*assign->left);
    ASSERT_NE(nullptr, x);
    EXPECT_EQ("x", x->identifier);

    // Right side of = is the top-level +
    const BinaryExpr* add1 = getBinary(*assign->right);
    ASSERT_NE(nullptr, add1);
    ASSERT_EQ(BinaryOperator::PLUS, add1->oper);

    // Left of that + should be another +
    const BinaryExpr* add2 = getBinary(*add1->left);
    ASSERT_NE(nullptr, add2);
    ASSERT_EQ(BinaryOperator::PLUS, add2->oper);

    // Right of inner + must be the * (higher precedence)
    const BinaryExpr* mul = getBinary(*add2->right);
    ASSERT_NE(nullptr, mul);
    ASSERT_EQ(BinaryOperator::MULT, mul->oper);

    const VarRefExpr* z = getVar(*mul->left);
    ASSERT_NE(nullptr, z);
    EXPECT_EQ("z", z->identifier);

    const IntLiteralExpr* two = getInt(*mul->right);
    ASSERT_NE(nullptr, two);
    EXPECT_EQ(2, two->value);
}

// --- Sizeof tests ---

static const SizeofExpr* getSizeof(const IExpression& e) {
    return std::get_if<SizeofExpr>(&e);
}

static bool holdsExpr(const ISizeofTerm& term) {
    return std::holds_alternative<std::unique_ptr<IExpression>>(term);
}

static bool holdsType(const ISizeofTerm& term) {
    return std::holds_alternative<TypeIdentifier>(term);
}

static const IExpression* getSizeofInnerExpr(const ISizeofTerm& term) {
    if (auto* p = std::get_if<std::unique_ptr<IExpression>>(&term)) {
        return p->get();
    }
    return nullptr;
}

static const TypeIdentifier* getSizeofTypeId(const ISizeofTerm& term) {
    return std::get_if<TypeIdentifier>(&term);
}

TEST(ParserTest, SizeofExpressionForms) {
    // sizeof x
    {
        auto tree = parse("int dummy = sizeof x;");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        ASSERT_NE(nullptr, varDecl);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsExpr(so->term));
        const VarRefExpr* v = getVar(*getSizeofInnerExpr(so->term));
        ASSERT_NE(nullptr, v);
        EXPECT_EQ("x", v->identifier);
    }

    // sizeof (x + 1)  -- parenthesized expression
    {
        auto tree = parse("int dummy = sizeof (x + 1);");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsExpr(so->term));
        const BinaryExpr* bin = getBinary(*getSizeofInnerExpr(so->term));
        ASSERT_NE(nullptr, bin);
        ASSERT_EQ(BinaryOperator::PLUS, bin->oper);
    }

    // sizeof x + 1   should be (sizeof x) + 1
    {
        auto tree = parse("int dummy = sizeof x + 1;");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const BinaryExpr* add = getBinary(*varDecl->expression);
        ASSERT_NE(nullptr, add);
        ASSERT_EQ(BinaryOperator::PLUS, add->oper);

        const SizeofExpr* so = getSizeof(*add->left);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsExpr(so->term));
    }

    // sizeof &p
    {
        auto tree = parse("int dummy = sizeof &p;");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsExpr(so->term));
        const UnaryExpr* un = std::get_if<UnaryExpr>(getSizeofInnerExpr(so->term));
        ASSERT_NE(nullptr, un);
        EXPECT_EQ(UnaryOperator::ADDROF, un->oper);
    }
}

TEST(ParserTest, SizeofTypeForms) {
    // sizeof (int)
    {
        auto tree = parse("int dummy = sizeof (int);");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsType(so->term));

        const TypeIdentifier* tid = getSizeofTypeId(so->term);
        ASSERT_NE(nullptr, tid);
        EXPECT_EQ(BaseType(BuiltInType::INT), tid->baseType);
        EXPECT_EQ(0, tid->pointerLevel);
        EXPECT_FALSE(tid->isStruct);
    }

    // sizeof (int *)
    {
        auto tree = parse("int dummy = sizeof (int *);");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsType(so->term));

        const TypeIdentifier* tid = getSizeofTypeId(so->term);
        ASSERT_NE(nullptr, tid);
        EXPECT_EQ(BaseType(BuiltInType::INT), tid->baseType);
        EXPECT_EQ(1, tid->pointerLevel);
    }

    // sizeof (struct foo)
    {
        auto tree = parse("int dummy = sizeof (struct foo);");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsType(so->term));

        const TypeIdentifier* tid = getSizeofTypeId(so->term);
        ASSERT_NE(nullptr, tid);
        EXPECT_EQ(BaseType("foo"), tid->baseType);
        EXPECT_TRUE(tid->isStruct);
        EXPECT_EQ(0, tid->pointerLevel);
    }

    // sizeof (const char **)
    {
        auto tree = parse("int dummy = sizeof (const char **);");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const SizeofExpr* so = getSizeof(*varDecl->expression);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsType(so->term));

        const TypeIdentifier* tid = getSizeofTypeId(so->term);
        ASSERT_NE(nullptr, tid);
        EXPECT_EQ(BaseType(BuiltInType::CHAR), tid->baseType);
        EXPECT_TRUE(tid->isConst);
        EXPECT_EQ(2, tid->pointerLevel);
    }

    // sizeof (int) * 4   -- multiplication must be outside the sizeof
    {
        auto tree = parse("int dummy = sizeof (int) * 4;");
        auto* varDecl = std::get_if<VariableDeclarationStatement>(&tree->rootItems[0]);
        const BinaryExpr* mul = getBinary(*varDecl->expression);
        ASSERT_NE(nullptr, mul);
        ASSERT_EQ(BinaryOperator::MULT, mul->oper);

        const SizeofExpr* so = getSizeof(*mul->left);
        ASSERT_NE(nullptr, so);
        ASSERT_TRUE(holdsType(so->term));
        const TypeIdentifier* tid = getSizeofTypeId(so->term);
        EXPECT_EQ(BaseType(BuiltInType::INT), tid->baseType);
    }
}


