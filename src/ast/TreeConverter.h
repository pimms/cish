#pragma once

#include "antlr/CMBaseVisitor.h"
#include "antlr/CMParser.h"
#include "AntlrContext.h"
#include "ParseContext.h"

#include "AstNodes.h"
#include "Lvalue.h"

#include "BinaryExpression.h"
#include "StringEscape.h"
#include "Ast.h"
#include "StringLiteralExpression.h"

#include "../module/ModuleContext.h"


namespace cish
{
namespace ast
{

using module::Module;
using module::ModuleContext;

DECLARE_EXCEPTION(AstNodeNotImplementedException);
DECLARE_EXCEPTION(AstConversionException);
DECLARE_EXCEPTION(FunctionNotDefinedException);
DECLARE_EXCEPTION(ModuleNotFoundException);

namespace internal
{


class TreeConverter: public CMBaseVisitor
{
    typedef std::vector<AstNode::Ptr> Result;

public:
    TreeConverter(ModuleContext::Ptr moduleContext);
    Ast::Ptr convertTree(const ParseContext *parseContext);

    virtual antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override;
    virtual antlrcpp::Any visitRoot(CMParser::RootContext *ctx) override;
    virtual antlrcpp::Any visitRootBlock(CMParser::RootBlockContext *ctx) override;
    virtual antlrcpp::Any visitRootItem(CMParser::RootItemContext *ctx) override;
    virtual antlrcpp::Any visitSystemInclude(CMParser::SystemIncludeContext *ctx) override;
    virtual antlrcpp::Any visitExpression(CMParser::ExpressionContext *ctx) override;
    virtual antlrcpp::Any manuallyVisitIncdecexpr(CMParser::IncdecexprContext *ctx);
    virtual antlrcpp::Any visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitSizeofTerm(CMParser::SizeofTermContext *ctx) override;
    virtual antlrcpp::Any visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitMULT_EXPR(CMParser::MULT_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitBITWISE_EXPR(CMParser::BITWISE_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitADD_EXPR(CMParser::ADD_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitAND_EXPR(CMParser::AND_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) override;
    virtual antlrcpp::Any visitStatement(CMParser::StatementContext *ctx) override;
    virtual antlrcpp::Any visitReturnStatement(CMParser::ReturnStatementContext *ctx) override;
    virtual antlrcpp::Any visitIfStatement(CMParser::IfStatementContext *ctx) override;
    virtual antlrcpp::Any visitElseStatement(CMParser::ElseStatementContext *ctx) override;
    virtual antlrcpp::Any visitForStatement(CMParser::ForStatementContext *ctx) override;
    virtual antlrcpp::Any visitWhileStatement(CMParser::WhileStatementContext *ctx) override;
    virtual antlrcpp::Any visitDoWhileStatement(CMParser::DoWhileStatementContext *ctx) override;
    virtual antlrcpp::Any visitExpressionStatement(CMParser::ExpressionStatementContext *ctx) override;
    virtual antlrcpp::Any visitAssignment(CMParser::AssignmentContext *ctx) override;
    virtual antlrcpp::Any visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) override;
    virtual antlrcpp::Any visitArithmeticAssignment(CMParser::ArithmeticAssignmentContext *ctx) override;
    virtual antlrcpp::Any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) override;
    virtual antlrcpp::Any visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) override;
    virtual antlrcpp::Any visitFunctionCall(CMParser::FunctionCallContext *ctx) override;
    virtual antlrcpp::Any visitExpressionList(CMParser::ExpressionListContext *ctx) override;
    virtual antlrcpp::Any visitIdentifierList(CMParser::IdentifierListContext *ctx) override;
    virtual antlrcpp::Any visitFunctionParameter(CMParser::FunctionParameterContext *ctx) override;
    virtual antlrcpp::Any visitStringLiteral(CMParser::StringLiteralContext *ctx) override;
    virtual antlrcpp::Any visitLvalVariableReference(CMParser::LvalVariableReferenceContext *ctx) override;
    virtual antlrcpp::Any visitLvalDereferencedVariable(CMParser::LvalDereferencedVariableContext *ctx) override;
    virtual antlrcpp::Any visitLvalSubscript(CMParser::LvalSubscriptContext *ctx) override;
    virtual antlrcpp::Any visitIdentifier(CMParser::IdentifierContext *ctx) override;
    virtual antlrcpp::Any visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) override;

private:
    DeclarationContext _declContext;
    StringTable::Ptr _stringTable;
    std::vector<FuncDeclaration> _funcDecls;
    ModuleContext::Ptr _moduleContext;
    std::set<std::string> _includedModules;

    Result createResult(AstNode *node);
    Result createResult(AstNode::Ptr node);
    Result buildBinaryExpression(BinaryExpression::Operator op, antlr4::tree::ParseTree *tree);

    Expression::Ptr castToExpression(AstNode::Ptr node);
    Statement::Ptr castToStatement(AstNode::Ptr node);
    Expression::Ptr manuallyVisitExpression(CMParser::ExpressionContext *ctx);
    Statement::Ptr manuallyVisitStatement(CMParser::StatementContext *ctx);
    std::vector<VarDeclaration> manuallyVisitIdentifierList(CMParser::IdentifierListContext *ctx);
    VarDeclaration manuallyVisitFunctionParameter(CMParser::FunctionParameterContext *ctx);
    StringLiteralExpression::Ptr manuallyVisitStringLiteral(CMParser::StringLiteralContext *ctx);
    Statement::Ptr manuallyVisitForInitializer(CMParser::ForInitializerContext *ctx);
    Statement::Ptr manuallyVisitForIterator(CMParser::ForIteratorContext *ctx);

    void verifyAllFunctionsDefined(Ast *ast);
    void includeModule(Ast *ast, std::string moduleName);
};


}
}
}
