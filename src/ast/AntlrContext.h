#pragma once

#include "ParseContext.h"
#include "LineNumberResolver.h"
#include "../Exception.h"

#include "antlr4-runtime.h"
#include "antlr/CMLexer.h"
#include "antlr/CMParser.h"

#include <string>


namespace cish
{
namespace ast
{


DECLARE_EXCEPTION(SyntaxErrorException);


class AntlrContext: public ParseContext, private antlr4::ANTLRErrorListener
{
public:
    typedef std::shared_ptr<AntlrContext> Ptr;

    AntlrContext(const std::string &source);
    ~AntlrContext();

    bool hasErrors() const override;
    std::vector<CompilationError> getErrors() const override;
    antlr4::tree::ParseTree* getParseTree() const override;

    uint32_t getLineNumber(uint32_t charIndex) const override;

private:
    antlr4::ANTLRInputStream *_inputStream;
    CMLexer *_lexer;
    antlr4::CommonTokenStream *_tokenStream;
    CMParser *_parser;
    antlr4::tree::ParseTree *_tree;
    LineNumberResolver _lineNumberResolver;

    std::vector<CompilationError> _errors;
    std::vector<uint32_t> _lineCharMap;


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
