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
    _tree(nullptr),
    _lineNumberResolver(source)
{
    _inputStream = new antlr4::ANTLRInputStream(source);

    _lexer = new CMLexer(_inputStream);
    _lexer->removeErrorListeners();
    _lexer->addErrorListener(this);

    _tokenStream = new antlr4::CommonTokenStream(_lexer);

    _parser = new CMParser(_tokenStream);
    _parser->removeErrorListeners();
    _parser->addErrorListener(this);

    _tree = _parser->root();
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

bool AntlrContext::hasErrors() const
{
    return _errors.size() != 0;
}

std::vector<CompilationError> AntlrContext::getErrors() const
{
    return _errors;
}

antlr4::tree::ParseTree* AntlrContext::getParseTree() const
{
    if (hasErrors()) {
        Throw(SyntaxErrorException, "Cannot get parse tree - there are syntax errors");
    }

    return _tree;
}

uint32_t AntlrContext::getLineNumber(uint32_t charIndex) const
{
    return _lineNumberResolver.resolveCharIndex(charIndex);
}


void AntlrContext::syntaxError(antlr4::Recognizer *recognizer,
                               antlr4::Token *offendingSymbol,
                               size_t line,
                               size_t charPositionInLine,
                               const std::string &msg,
                               std::exception_ptr e)
{
    CompilationError err;
    err.lineNumber = line;
    err.charNumber = charPositionInLine;
    err.message = msg;
    _errors.push_back(err);
}

void AntlrContext::reportAmbiguity(antlr4::Parser *recognizer,
                                   const antlr4::dfa::DFA &dfa,
                                   size_t startIndex,
                                   size_t stopIndex,
                                   bool exact,
                                   const antlrcpp::BitSet &ambigAlts,
                                   antlr4::atn::ATNConfigSet *configs)
{
    // fprintf(stderr, "ANTLR4 Ambiguity!\n");
}

void AntlrContext::reportAttemptingFullContext(antlr4::Parser *recognizer,
                                               const antlr4::dfa::DFA &dfa,
                                               size_t startIndex,
                                               size_t stopIndex,
                                               const antlrcpp::BitSet &conflictingAlts,
                                               antlr4::atn::ATNConfigSet *configs)
{
    // fprintf(stderr, "ANTLR4 Attempting full context (?)\n");
}

void AntlrContext::reportContextSensitivity(antlr4::Parser *recognizer,
                                            const antlr4::dfa::DFA &dfa,
                                            size_t startIndex,
                                            size_t stopIndex,
                                            size_t prediction,
                                            antlr4::atn::ATNConfigSet *configs)
{
    // fprintf(stderr, "ANTLR4 Context sensitivity (?)\n");
}


}
}
