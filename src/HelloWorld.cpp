#include <iostream>
#include <fstream>

#include "antlr4-runtime.h"
#include "ast/antlr/CMLexer.h"
#include "ast/antlr/CMParser.h"

namespace cish
{

void foo()
{
    antlr4::ANTLRInputStream inputStream("const int CONSTANT = 15;");

    CMLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);

    CMParser parser(&tokenStream);

    antlr4::tree::ParseTree *tree = parser.root();
    std::cout << tree->toStringTree(&parser) << "\n";
}

}

