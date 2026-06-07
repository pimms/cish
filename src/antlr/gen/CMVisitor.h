
// Generated from /Users/joakimstien/code/cish/cish/grammar/CM.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CMParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CMParser.
 */
class  CMVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CMParser.
   */
    virtual std::any visitRoot(CMParser::RootContext *context) = 0;

    virtual std::any visitRootBlock(CMParser::RootBlockContext *context) = 0;

    virtual std::any visitRootItem(CMParser::RootItemContext *context) = 0;

    virtual std::any visitExpression(CMParser::ExpressionContext *context) = 0;

    virtual std::any visitMINUS_EXPR(CMParser::MINUS_EXPRContext *context) = 0;

    virtual std::any visitMULT_EXPR(CMParser::MULT_EXPRContext *context) = 0;

    virtual std::any visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *context) = 0;

    virtual std::any visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *context) = 0;

    virtual std::any visitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *context) = 0;

    virtual std::any visitADD_EXPR(CMParser::ADD_EXPRContext *context) = 0;

    virtual std::any visitAND_EXPR(CMParser::AND_EXPRContext *context) = 0;

    virtual std::any visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *context) = 0;

    virtual std::any visitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *context) = 0;

    virtual std::any visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *context) = 0;

    virtual std::any visitBITWISE_EXPR(CMParser::BITWISE_EXPRContext *context) = 0;

    virtual std::any visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *context) = 0;

    virtual std::any visitDEREF_EXPR(CMParser::DEREF_EXPRContext *context) = 0;

    virtual std::any visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *context) = 0;

    virtual std::any visitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *context) = 0;

    virtual std::any visitEXPR_ATOM___(CMParser::EXPR_ATOM___Context *context) = 0;

    virtual std::any visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *context) = 0;

    virtual std::any visitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *context) = 0;

    virtual std::any visitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *context) = 0;

    virtual std::any visitPAREN_EXPR___(CMParser::PAREN_EXPR___Context *context) = 0;

    virtual std::any visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *context) = 0;

    virtual std::any visitSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext *context) = 0;

    virtual std::any visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *context) = 0;

    virtual std::any visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *context) = 0;

    virtual std::any visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *context) = 0;

    virtual std::any visitSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext *context) = 0;

    virtual std::any visitStatement(CMParser::StatementContext *context) = 0;

    virtual std::any visitExpressionStatement(CMParser::ExpressionStatementContext *context) = 0;

    virtual std::any visitReturnStatement(CMParser::ReturnStatementContext *context) = 0;

    virtual std::any visitIfStatement(CMParser::IfStatementContext *context) = 0;

    virtual std::any visitElseStatement(CMParser::ElseStatementContext *context) = 0;

    virtual std::any visitForStatement(CMParser::ForStatementContext *context) = 0;

    virtual std::any visitForInitializer(CMParser::ForInitializerContext *context) = 0;

    virtual std::any visitForIterator(CMParser::ForIteratorContext *context) = 0;

    virtual std::any visitWhileStatement(CMParser::WhileStatementContext *context) = 0;

    virtual std::any visitDoWhileStatement(CMParser::DoWhileStatementContext *context) = 0;

    virtual std::any visitAssignment(CMParser::AssignmentContext *context) = 0;

    virtual std::any visitArithmeticAssignment(CMParser::ArithmeticAssignmentContext *context) = 0;

    virtual std::any visitVariableDeclaration(CMParser::VariableDeclarationContext *context) = 0;

    virtual std::any visitStructDeclaration(CMParser::StructDeclarationContext *context) = 0;

    virtual std::any visitStructFieldDeclaration(CMParser::StructFieldDeclarationContext *context) = 0;

    virtual std::any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *context) = 0;

    virtual std::any visitFunctionDefinition(CMParser::FunctionDefinitionContext *context) = 0;

    virtual std::any visitFunctionCall(CMParser::FunctionCallContext *context) = 0;

    virtual std::any visitExpressionList(CMParser::ExpressionListContext *context) = 0;

    virtual std::any visitIdentifierList(CMParser::IdentifierListContext *context) = 0;

    virtual std::any visitFunctionParameter(CMParser::FunctionParameterContext *context) = 0;

    virtual std::any visitIdentifier(CMParser::IdentifierContext *context) = 0;

    virtual std::any visitTypeIdentifier(CMParser::TypeIdentifierContext *context) = 0;

    virtual std::any visitTypeName(CMParser::TypeNameContext *context) = 0;

    virtual std::any visitSizeofTerm(CMParser::SizeofTermContext *context) = 0;

    virtual std::any visitStringLiteral(CMParser::StringLiteralContext *context) = 0;

    virtual std::any visitSystemInclude(CMParser::SystemIncludeContext *context) = 0;

    virtual std::any visitSysModuleName(CMParser::SysModuleNameContext *context) = 0;


};

