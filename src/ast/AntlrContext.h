#pragma once

#include <string>

#include "antlr4-runtime.h"
#include "antlr/CMLexer.h"
#include "antlr/CMParser.h"


namespace cish
{
namespace ast
{


class AntlrContext
{
public:
    AntlrContext(const std::string &source);
    ~AntlrContext();

    antlr4::tree::ParseTree* getParseTree() const;

private:
    antlr4::ANTLRInputStream *_inputStream;
    CMLexer *_lexer;
    antlr4::CommonTokenStream *_tokenStream;
    CMParser *_parser;
    antlr4::tree::ParseTree *_tree;
};


}
}
