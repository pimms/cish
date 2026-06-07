
// Generated from /Users/joakimstien/code/cish/cish/grammar/CM.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CMListener.h"


/**
 * This class provides an empty implementation of CMListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  CMBaseListener : public CMListener {
public:

  virtual void enterRoot(CMParser::RootContext * /*ctx*/) override { }
  virtual void exitRoot(CMParser::RootContext * /*ctx*/) override { }

  virtual void enterRootBlock(CMParser::RootBlockContext * /*ctx*/) override { }
  virtual void exitRootBlock(CMParser::RootBlockContext * /*ctx*/) override { }

  virtual void enterRootItem(CMParser::RootItemContext * /*ctx*/) override { }
  virtual void exitRootItem(CMParser::RootItemContext * /*ctx*/) override { }

  virtual void enterExpression(CMParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(CMParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterMINUS_EXPR(CMParser::MINUS_EXPRContext * /*ctx*/) override { }
  virtual void exitMINUS_EXPR(CMParser::MINUS_EXPRContext * /*ctx*/) override { }

  virtual void enterMULT_EXPR(CMParser::MULT_EXPRContext * /*ctx*/) override { }
  virtual void exitMULT_EXPR(CMParser::MULT_EXPRContext * /*ctx*/) override { }

  virtual void enterNEGATION_EXPR(CMParser::NEGATION_EXPRContext * /*ctx*/) override { }
  virtual void exitNEGATION_EXPR(CMParser::NEGATION_EXPRContext * /*ctx*/) override { }

  virtual void enterPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext * /*ctx*/) override { }
  virtual void exitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext * /*ctx*/) override { }

  virtual void enterONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext * /*ctx*/) override { }
  virtual void exitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext * /*ctx*/) override { }

  virtual void enterADD_EXPR(CMParser::ADD_EXPRContext * /*ctx*/) override { }
  virtual void exitADD_EXPR(CMParser::ADD_EXPRContext * /*ctx*/) override { }

  virtual void enterAND_EXPR(CMParser::AND_EXPRContext * /*ctx*/) override { }
  virtual void exitAND_EXPR(CMParser::AND_EXPRContext * /*ctx*/) override { }

  virtual void enterPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext * /*ctx*/) override { }
  virtual void exitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext * /*ctx*/) override { }

  virtual void enterSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext * /*ctx*/) override { }
  virtual void exitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext * /*ctx*/) override { }

  virtual void enterADDROF_EXPR(CMParser::ADDROF_EXPRContext * /*ctx*/) override { }
  virtual void exitADDROF_EXPR(CMParser::ADDROF_EXPRContext * /*ctx*/) override { }

  virtual void enterBITWISE_EXPR(CMParser::BITWISE_EXPRContext * /*ctx*/) override { }
  virtual void exitBITWISE_EXPR(CMParser::BITWISE_EXPRContext * /*ctx*/) override { }

  virtual void enterPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext * /*ctx*/) override { }
  virtual void exitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext * /*ctx*/) override { }

  virtual void enterDEREF_EXPR(CMParser::DEREF_EXPRContext * /*ctx*/) override { }
  virtual void exitDEREF_EXPR(CMParser::DEREF_EXPRContext * /*ctx*/) override { }

  virtual void enterEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext * /*ctx*/) override { }
  virtual void exitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext * /*ctx*/) override { }

  virtual void enterSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext * /*ctx*/) override { }
  virtual void exitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext * /*ctx*/) override { }

  virtual void enterEXPR_ATOM___(CMParser::EXPR_ATOM___Context * /*ctx*/) override { }
  virtual void exitEXPR_ATOM___(CMParser::EXPR_ATOM___Context * /*ctx*/) override { }

  virtual void enterPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext * /*ctx*/) override { }
  virtual void exitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext * /*ctx*/) override { }

  virtual void enterTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext * /*ctx*/) override { }
  virtual void exitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext * /*ctx*/) override { }

  virtual void enterBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext * /*ctx*/) override { }
  virtual void exitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext * /*ctx*/) override { }

  virtual void enterPAREN_EXPR___(CMParser::PAREN_EXPR___Context * /*ctx*/) override { }
  virtual void exitPAREN_EXPR___(CMParser::PAREN_EXPR___Context * /*ctx*/) override { }

  virtual void enterCOMPARE_EXPR(CMParser::COMPARE_EXPRContext * /*ctx*/) override { }
  virtual void exitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext * /*ctx*/) override { }

  virtual void enterSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext * /*ctx*/) override { }
  virtual void exitSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext * /*ctx*/) override { }

  virtual void enterLITERAL_EXPR(CMParser::LITERAL_EXPRContext * /*ctx*/) override { }
  virtual void exitLITERAL_EXPR(CMParser::LITERAL_EXPRContext * /*ctx*/) override { }

  virtual void enterVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext * /*ctx*/) override { }
  virtual void exitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext * /*ctx*/) override { }

  virtual void enterFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext * /*ctx*/) override { }
  virtual void exitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext * /*ctx*/) override { }

  virtual void enterSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext * /*ctx*/) override { }
  virtual void exitSTR_LITERAL_EXPR(CMParser::STR_LITERAL_EXPRContext * /*ctx*/) override { }

  virtual void enterStatement(CMParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(CMParser::StatementContext * /*ctx*/) override { }

  virtual void enterExpressionStatement(CMParser::ExpressionStatementContext * /*ctx*/) override { }
  virtual void exitExpressionStatement(CMParser::ExpressionStatementContext * /*ctx*/) override { }

  virtual void enterReturnStatement(CMParser::ReturnStatementContext * /*ctx*/) override { }
  virtual void exitReturnStatement(CMParser::ReturnStatementContext * /*ctx*/) override { }

  virtual void enterIfStatement(CMParser::IfStatementContext * /*ctx*/) override { }
  virtual void exitIfStatement(CMParser::IfStatementContext * /*ctx*/) override { }

  virtual void enterElseStatement(CMParser::ElseStatementContext * /*ctx*/) override { }
  virtual void exitElseStatement(CMParser::ElseStatementContext * /*ctx*/) override { }

  virtual void enterForStatement(CMParser::ForStatementContext * /*ctx*/) override { }
  virtual void exitForStatement(CMParser::ForStatementContext * /*ctx*/) override { }

  virtual void enterForInitializer(CMParser::ForInitializerContext * /*ctx*/) override { }
  virtual void exitForInitializer(CMParser::ForInitializerContext * /*ctx*/) override { }

  virtual void enterForIterator(CMParser::ForIteratorContext * /*ctx*/) override { }
  virtual void exitForIterator(CMParser::ForIteratorContext * /*ctx*/) override { }

  virtual void enterWhileStatement(CMParser::WhileStatementContext * /*ctx*/) override { }
  virtual void exitWhileStatement(CMParser::WhileStatementContext * /*ctx*/) override { }

  virtual void enterDoWhileStatement(CMParser::DoWhileStatementContext * /*ctx*/) override { }
  virtual void exitDoWhileStatement(CMParser::DoWhileStatementContext * /*ctx*/) override { }

  virtual void enterAssignment(CMParser::AssignmentContext * /*ctx*/) override { }
  virtual void exitAssignment(CMParser::AssignmentContext * /*ctx*/) override { }

  virtual void enterArithmeticAssignment(CMParser::ArithmeticAssignmentContext * /*ctx*/) override { }
  virtual void exitArithmeticAssignment(CMParser::ArithmeticAssignmentContext * /*ctx*/) override { }

  virtual void enterVariableDeclaration(CMParser::VariableDeclarationContext * /*ctx*/) override { }
  virtual void exitVariableDeclaration(CMParser::VariableDeclarationContext * /*ctx*/) override { }

  virtual void enterStructDeclaration(CMParser::StructDeclarationContext * /*ctx*/) override { }
  virtual void exitStructDeclaration(CMParser::StructDeclarationContext * /*ctx*/) override { }

  virtual void enterStructFieldDeclaration(CMParser::StructFieldDeclarationContext * /*ctx*/) override { }
  virtual void exitStructFieldDeclaration(CMParser::StructFieldDeclarationContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(CMParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(CMParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterFunctionDefinition(CMParser::FunctionDefinitionContext * /*ctx*/) override { }
  virtual void exitFunctionDefinition(CMParser::FunctionDefinitionContext * /*ctx*/) override { }

  virtual void enterFunctionCall(CMParser::FunctionCallContext * /*ctx*/) override { }
  virtual void exitFunctionCall(CMParser::FunctionCallContext * /*ctx*/) override { }

  virtual void enterExpressionList(CMParser::ExpressionListContext * /*ctx*/) override { }
  virtual void exitExpressionList(CMParser::ExpressionListContext * /*ctx*/) override { }

  virtual void enterIdentifierList(CMParser::IdentifierListContext * /*ctx*/) override { }
  virtual void exitIdentifierList(CMParser::IdentifierListContext * /*ctx*/) override { }

  virtual void enterFunctionParameter(CMParser::FunctionParameterContext * /*ctx*/) override { }
  virtual void exitFunctionParameter(CMParser::FunctionParameterContext * /*ctx*/) override { }

  virtual void enterIdentifier(CMParser::IdentifierContext * /*ctx*/) override { }
  virtual void exitIdentifier(CMParser::IdentifierContext * /*ctx*/) override { }

  virtual void enterTypeIdentifier(CMParser::TypeIdentifierContext * /*ctx*/) override { }
  virtual void exitTypeIdentifier(CMParser::TypeIdentifierContext * /*ctx*/) override { }

  virtual void enterTypeName(CMParser::TypeNameContext * /*ctx*/) override { }
  virtual void exitTypeName(CMParser::TypeNameContext * /*ctx*/) override { }

  virtual void enterSizeofTerm(CMParser::SizeofTermContext * /*ctx*/) override { }
  virtual void exitSizeofTerm(CMParser::SizeofTermContext * /*ctx*/) override { }

  virtual void enterStringLiteral(CMParser::StringLiteralContext * /*ctx*/) override { }
  virtual void exitStringLiteral(CMParser::StringLiteralContext * /*ctx*/) override { }

  virtual void enterSystemInclude(CMParser::SystemIncludeContext * /*ctx*/) override { }
  virtual void exitSystemInclude(CMParser::SystemIncludeContext * /*ctx*/) override { }

  virtual void enterSysModuleName(CMParser::SysModuleNameContext * /*ctx*/) override { }
  virtual void exitSysModuleName(CMParser::SysModuleNameContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

