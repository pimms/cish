
// Generated from /Users/joakimstien/code/cish/cish/grammar/CM.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CMParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by CMParser.
 */
class  CMListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterRoot(CMParser::RootContext *ctx) = 0;
  virtual void exitRoot(CMParser::RootContext *ctx) = 0;

  virtual void enterRootBlock(CMParser::RootBlockContext *ctx) = 0;
  virtual void exitRootBlock(CMParser::RootBlockContext *ctx) = 0;

  virtual void enterRootItem(CMParser::RootItemContext *ctx) = 0;
  virtual void exitRootItem(CMParser::RootItemContext *ctx) = 0;

  virtual void enterExpression(CMParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(CMParser::ExpressionContext *ctx) = 0;

  virtual void enterMINUS_EXPR(CMParser::MINUS_EXPRContext *ctx) = 0;
  virtual void exitMINUS_EXPR(CMParser::MINUS_EXPRContext *ctx) = 0;

  virtual void enterMULT_EXPR(CMParser::MULT_EXPRContext *ctx) = 0;
  virtual void exitMULT_EXPR(CMParser::MULT_EXPRContext *ctx) = 0;

  virtual void enterNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx) = 0;
  virtual void exitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx) = 0;

  virtual void enterPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx) = 0;
  virtual void exitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx) = 0;

  virtual void enterONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *ctx) = 0;
  virtual void exitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *ctx) = 0;

  virtual void enterADD_EXPR(CMParser::ADD_EXPRContext *ctx) = 0;
  virtual void exitADD_EXPR(CMParser::ADD_EXPRContext *ctx) = 0;

  virtual void enterAND_EXPR(CMParser::AND_EXPRContext *ctx) = 0;
  virtual void exitAND_EXPR(CMParser::AND_EXPRContext *ctx) = 0;

  virtual void enterPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx) = 0;
  virtual void exitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx) = 0;

  virtual void enterSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *ctx) = 0;
  virtual void exitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *ctx) = 0;

  virtual void enterADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx) = 0;
  virtual void exitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx) = 0;

  virtual void enterBITWISE_EXPR(CMParser::BITWISE_EXPRContext *ctx) = 0;
  virtual void exitBITWISE_EXPR(CMParser::BITWISE_EXPRContext *ctx) = 0;

  virtual void enterPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx) = 0;
  virtual void exitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx) = 0;

  virtual void enterDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx) = 0;
  virtual void exitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx) = 0;

  virtual void enterEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) = 0;
  virtual void exitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) = 0;

  virtual void enterSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *ctx) = 0;
  virtual void exitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *ctx) = 0;

  virtual void enterEXPR_ATOM___(CMParser::EXPR_ATOM___Context *ctx) = 0;
  virtual void exitEXPR_ATOM___(CMParser::EXPR_ATOM___Context *ctx) = 0;

  virtual void enterPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx) = 0;
  virtual void exitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx) = 0;

  virtual void enterTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *ctx) = 0;
  virtual void exitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *ctx) = 0;

  virtual void enterBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *ctx) = 0;
  virtual void exitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *ctx) = 0;

  virtual void enterPAREN_EXPR___(CMParser::PAREN_EXPR___Context *ctx) = 0;
  virtual void exitPAREN_EXPR___(CMParser::PAREN_EXPR___Context *ctx) = 0;

  virtual void enterCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) = 0;
  virtual void exitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) = 0;

  virtual void enterSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext *ctx) = 0;
  virtual void exitSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext *ctx) = 0;

  virtual void enterLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) = 0;
  virtual void exitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) = 0;

  virtual void enterVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) = 0;
  virtual void exitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) = 0;

  virtual void enterFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) = 0;
  virtual void exitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) = 0;

  virtual void enterSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext *ctx) = 0;
  virtual void exitSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext *ctx) = 0;

  virtual void enterStatement(CMParser::StatementContext *ctx) = 0;
  virtual void exitStatement(CMParser::StatementContext *ctx) = 0;

  virtual void enterExpressionStatement(CMParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(CMParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterReturnStatement(CMParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(CMParser::ReturnStatementContext *ctx) = 0;

  virtual void enterIfStatement(CMParser::IfStatementContext *ctx) = 0;
  virtual void exitIfStatement(CMParser::IfStatementContext *ctx) = 0;

  virtual void enterElseStatement(CMParser::ElseStatementContext *ctx) = 0;
  virtual void exitElseStatement(CMParser::ElseStatementContext *ctx) = 0;

  virtual void enterForStatement(CMParser::ForStatementContext *ctx) = 0;
  virtual void exitForStatement(CMParser::ForStatementContext *ctx) = 0;

  virtual void enterForInitializer(CMParser::ForInitializerContext *ctx) = 0;
  virtual void exitForInitializer(CMParser::ForInitializerContext *ctx) = 0;

  virtual void enterForIterator(CMParser::ForIteratorContext *ctx) = 0;
  virtual void exitForIterator(CMParser::ForIteratorContext *ctx) = 0;

  virtual void enterWhileStatement(CMParser::WhileStatementContext *ctx) = 0;
  virtual void exitWhileStatement(CMParser::WhileStatementContext *ctx) = 0;

  virtual void enterDoWhileStatement(CMParser::DoWhileStatementContext *ctx) = 0;
  virtual void exitDoWhileStatement(CMParser::DoWhileStatementContext *ctx) = 0;

  virtual void enterAssignment(CMParser::AssignmentContext *ctx) = 0;
  virtual void exitAssignment(CMParser::AssignmentContext *ctx) = 0;

  virtual void enterArithmeticAssignment(CMParser::ArithmeticAssignmentContext *ctx) = 0;
  virtual void exitArithmeticAssignment(CMParser::ArithmeticAssignmentContext *ctx) = 0;

  virtual void enterVariableDeclaration(CMParser::VariableDeclarationContext *ctx) = 0;
  virtual void exitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) = 0;

  virtual void enterStructDeclaration(CMParser::StructDeclarationContext *ctx) = 0;
  virtual void exitStructDeclaration(CMParser::StructDeclarationContext *ctx) = 0;

  virtual void enterStructFieldDeclaration(CMParser::StructFieldDeclarationContext *ctx) = 0;
  virtual void exitStructFieldDeclaration(CMParser::StructFieldDeclarationContext *ctx) = 0;

  virtual void enterFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) = 0;
  virtual void exitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) = 0;

  virtual void enterFunctionCall(CMParser::FunctionCallContext *ctx) = 0;
  virtual void exitFunctionCall(CMParser::FunctionCallContext *ctx) = 0;

  virtual void enterExpressionList(CMParser::ExpressionListContext *ctx) = 0;
  virtual void exitExpressionList(CMParser::ExpressionListContext *ctx) = 0;

  virtual void enterIdentifierList(CMParser::IdentifierListContext *ctx) = 0;
  virtual void exitIdentifierList(CMParser::IdentifierListContext *ctx) = 0;

  virtual void enterFunctionParameter(CMParser::FunctionParameterContext *ctx) = 0;
  virtual void exitFunctionParameter(CMParser::FunctionParameterContext *ctx) = 0;

  virtual void enterIdentifier(CMParser::IdentifierContext *ctx) = 0;
  virtual void exitIdentifier(CMParser::IdentifierContext *ctx) = 0;

  virtual void enterTypeIdentifier(CMParser::TypeIdentifierContext *ctx) = 0;
  virtual void exitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) = 0;

  virtual void enterTypeName(CMParser::TypeNameContext *ctx) = 0;
  virtual void exitTypeName(CMParser::TypeNameContext *ctx) = 0;

  virtual void enterSizeofTerm(CMParser::SizeofTermContext *ctx) = 0;
  virtual void exitSizeofTerm(CMParser::SizeofTermContext *ctx) = 0;

  virtual void enterStringLiteral(CMParser::StringLiteralContext *ctx) = 0;
  virtual void exitStringLiteral(CMParser::StringLiteralContext *ctx) = 0;

  virtual void enterSystemInclude(CMParser::SystemIncludeContext *ctx) = 0;
  virtual void exitSystemInclude(CMParser::SystemIncludeContext *ctx) = 0;

  virtual void enterSysModuleName(CMParser::SysModuleNameContext *ctx) = 0;
  virtual void exitSysModuleName(CMParser::SysModuleNameContext *ctx) = 0;


};

