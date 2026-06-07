
// Generated from /Users/joakimstien/code/cish/cish/grammar/CM.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CMVisitor.h"


/**
 * This class provides an empty implementation of CMVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CMBaseVisitor : public CMVisitor {
public:

  virtual std::any visitRoot(CMParser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRootBlock(CMParser::RootBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRootItem(CMParser::RootItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(CMParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMINUS_EXPR(CMParser::MINUS_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMULT_EXPR(CMParser::MULT_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitADD_EXPR(CMParser::ADD_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAND_EXPR(CMParser::AND_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBITWISE_EXPR(CMParser::BITWISE_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEXPR_ATOM___(CMParser::EXPR_ATOM___Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPAREN_EXPR___(CMParser::PAREN_EXPR___Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(CMParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionStatement(CMParser::ExpressionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStatement(CMParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStatement(CMParser::IfStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElseStatement(CMParser::ElseStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForStatement(CMParser::ForStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForInitializer(CMParser::ForInitializerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForIterator(CMParser::ForIteratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStatement(CMParser::WhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDoWhileStatement(CMParser::DoWhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignment(CMParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArithmeticAssignment(CMParser::ArithmeticAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclaration(CMParser::StructDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructFieldDeclaration(CMParser::StructFieldDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionCall(CMParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionList(CMParser::ExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdentifierList(CMParser::IdentifierListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionParameter(CMParser::FunctionParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdentifier(CMParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeName(CMParser::TypeNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSizeofTerm(CMParser::SizeofTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStringLiteral(CMParser::StringLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSystemInclude(CMParser::SystemIncludeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSysModuleName(CMParser::SysModuleNameContext *ctx) override {
    return visitChildren(ctx);
  }


};

