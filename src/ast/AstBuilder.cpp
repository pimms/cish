#include "AstBuilder.h"
#include "TreeConverter.h"

#include "antlr/CMBaseVisitor.h"
#include "AntlrContext.h"
#include "ParseContext.h"

#include <cassert>


namespace cish
{
namespace ast
{


AstBuilder::AstBuilder(const ParseContext::Ptr parseContext, ModuleContext::Ptr moduleContext):
    _parseContext(parseContext),
    _moduleContext(std::move(moduleContext))
{
}

Ast::Ptr AstBuilder::buildAst()
{
    // TODO: Propagate these errors out of here in a more reasonable way
    if (_parseContext->hasErrors()) {
        std::vector<ast::CompilationError> errors = _parseContext->getErrors();

        std::stringstream ss;
        ss << "There are syntax errors:";

        for (auto err: errors) {
            ss  << std::endl
                << "Syntax error on line "
                << err.lineNumber << ", near char "
                << err.charNumber << ":" << err.message;
        }

        std::string errorStr = ss.str();
        Throw(SyntaxErrorException, errorStr.c_str());
    }

    internal::TreeConverter converter(std::move(_moduleContext));
    Ast::Ptr ast = converter.convertTree(_parseContext.get());
    return std::move(ast);
}


}
}
