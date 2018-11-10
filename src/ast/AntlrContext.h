#pragma once

#include <string>

#include "antlr4-runtime.h"
#include "antlr/CMLexer.h"
#include "antlr/CMParser.h"

#include "../Exception.h"

namespace cish
{
namespace ast
{

DECLARE_EXCEPTION(SyntaxErrorException);


struct CompilationError
{
    std::string message;
    int lineNumber;
    int charNumber;
};


class AntlrContext: private antlr4::ANTLRErrorListener
{
public:

    AntlrContext(const std::string &source);
    ~AntlrContext();

    bool hasErrors() const;
    std::vector<CompilationError> getErrors() const;

    antlr4::tree::ParseTree* getParseTree() const;

private:
    antlr4::ANTLRInputStream *_inputStream;
    CMLexer *_lexer;
    antlr4::CommonTokenStream *_tokenStream;
    CMParser *_parser;
    antlr4::tree::ParseTree *_tree;

    std::vector<CompilationError> _errors;


    virtual void syntaxError(antlr4::Recognizer *recognizer,
                             antlr4::Token *offendingSymbol,
                             size_t line,
                             size_t charPositionInLine,
                             const std::string &msg,
                             std::exception_ptr e) override;
    virtual void reportAmbiguity(antlr4::Parser *recognizer,
                                 const antlr4::dfa::DFA &dfa,
                                 size_t startIndex,
                                 size_t stopIndex,
                                 bool exact,
                                 const antlrcpp::BitSet &ambigAlts,
                                 antlr4::atn::ATNConfigSet *configs) override;
    virtual void reportAttemptingFullContext(antlr4::Parser *recognizer,
                                             const antlr4::dfa::DFA &dfa,
                                             size_t startIndex,
                                             size_t stopIndex,
                                             const antlrcpp::BitSet &conflictingAlts,
                                             antlr4::atn::ATNConfigSet *configs) override;
    virtual void reportContextSensitivity(antlr4::Parser *recognizer,
                                          const antlr4::dfa::DFA &dfa,
                                          size_t startIndex,
                                          size_t stopIndex,
                                          size_t prediction,
                                          antlr4::atn::ATNConfigSet *configs) override;
};


}
}
