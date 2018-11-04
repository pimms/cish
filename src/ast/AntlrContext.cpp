#include "AntlrContext.h"


namespace cish
{
namespace ast
{


AntlrContext::AntlrContext(const std::string &source):
    _inputStream(nullptr),
    _lexer(nullptr),
    _tokenStream(nullptr),
    _parser(nullptr),
    _tree(nullptr)
{
    _inputStream = new antlr4::ANTLRInputStream(source);
    _lexer = new CMLexer(_inputStream);
    _tokenStream = new antlr4::CommonTokenStream(_lexer);
    _parser = new CMParser(_tokenStream);
    _tree = _parser->root();
    
    printf("Tree: %s\n", _tree->toStringTree(_parser).c_str());
}

AntlrContext::~AntlrContext()
{
    if (_parser)
        delete _parser;
    if (_tokenStream)
        delete _tokenStream;
    if (_lexer)
        delete _lexer;
    if (_inputStream)
        delete _inputStream;
    _tree = nullptr;
}

antlr4::tree::ParseTree* AntlrContext::getParseTree() const
{
    return _tree;
}


}
}
