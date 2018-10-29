#pragma once

#include "antlr/CMBaseVisitor.h"

namespace cish
{
namespace ast
{

class ParseVisitor: public CMBaseVisitor
{
public:
    virtual antlrcpp::Any visitRoot(CMParser::RootContext *ctx) override {
        printf("root (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitRootBlock(CMParser::RootBlockContext *ctx) override {
        printf("root block (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitRootItem(CMParser::RootItemContext *ctx) override {
        printf("root item (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitExpression(CMParser::ExpressionContext *ctx) override {
        printf("expression (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitMULT_EXPR(CMParser::MULT_EXPRContext *ctx) override {
        printf("multiply: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) override {
        printf("equality: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) override {
        printf("literal: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) override {
        printf("function call: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitADD_EXPR(CMParser::ADD_EXPRContext *ctx) override {
        printf("add call: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitAND_EXPR(CMParser::AND_EXPRContext *ctx) override {
        printf("and call: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) override {
        printf("var ref expr: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) override {
        printf("compare: (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitStatement(CMParser::StatementContext *ctx) override {
        printf("statement (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitReturnStatement(CMParser::ReturnStatementContext *ctx) override {
        printf("return statement (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitIfStatement(CMParser::IfStatementContext *ctx) override {
        printf("if statement (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitElseStatement(CMParser::ElseStatementContext *ctx) override {
        printf("else statement (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitAssignment(CMParser::AssignmentContext *ctx) override {
        printf("variable assignment (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) override {
        printf("variable declaration (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) override {
        printf("function declaration (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) override {
        printf("function definition (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFunctionCall(CMParser::FunctionCallContext *ctx) override {
        printf("function call (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitExpressionList(CMParser::ExpressionListContext *ctx) override {
        printf("expression list (%s)\n", ctx->toString().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitIdentifier(CMParser::IdentifierContext *ctx) override {
        printf("identifier (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitIdentifierList(CMParser::IdentifierListContext *ctx) override {
        printf("identifier list (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) override {
        printf("type identifier (%s)\n", ctx->getText().c_str());
        return visitChildren(ctx);
    }
};

}
}
