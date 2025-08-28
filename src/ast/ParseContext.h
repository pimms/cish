#pragma once

#include <string>
#include <vector>

namespace antlr4::tree
{
class ParseTree;
}

namespace cish::ast
{

struct CompilationError
{
    std::string message;
    int lineNumber;
    int charNumber;
};

/**
 * Note: ParseContext exists to abstract away our ANTLR-dependency
 * from our dependants.
 */
class ParseContext
{
public:
    typedef std::shared_ptr<ParseContext> Ptr;

    static Ptr parseSource(const std::string &source);

    virtual bool hasErrors() const = 0;
    virtual std::vector<CompilationError> getErrors() const = 0;
    virtual antlr4::tree::ParseTree* getParseTree() const = 0;
};

}
