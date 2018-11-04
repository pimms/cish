#include <gtest/gtest.h>

#include "ast/AntlrContext.h"

using namespace cish::ast;

TEST(AntlrContextTest, invalidSyntaxThrowsException)
{
    std::vector<std::string> invalidSyntax = {
        "int a = ;",
        "int b",
        "hei mamma",
        "int a (= 5);"
    };
    
    for (auto source : invalidSyntax) {
        AntlrContext context(source);
        ASSERT_TRUE(context.hasErrors());
        ASSERT_THROW(context.getParseTree(), SyntaxErrorException);
    }
}
