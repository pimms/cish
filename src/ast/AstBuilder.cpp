#include "AstBuilder.h"

#include "antlr/CMBaseVisitor.h"
#include "AntlrContext.h"

#include "AstNodes.h"

#include "BinaryExpression.h"
#include "VariableReferenceExpression.h"
#include "FunctionCallExpression.h"
#include "LiteralExpression.h"

#include "VariableAssignmentStatement.h"
#include "VariableDeclarationStatement.h"
#include "FunctionDeclarationStatement.h"
#include "FunctionDefinition.h"
#include "FunctionCallStatement.h"
#include "ReturnStatement.h"

#include <cassert>


namespace cish
{
namespace ast
{

namespace internal
{

DECLARE_EXCEPTION(AstNodeNotImplementedException);
DECLARE_EXCEPTION(AstConversionException);

class TreeConverter: public CMBaseVisitor
{
    typedef std::vector<AstNode*> Result;
    DeclarationContext _declContext;

    Result createResult(AstNode *node)
    {
        return Result { node };
    }

    Expression* manuallyVisitExpression(CMParser::ExpressionContext *ctx)
    {
        antlrcpp::Any any = visitExpression(ctx);
        assert(any.isNotNull());

        Result result = any.as<Result>();
        assert(result.size() == 1);
        assert(dynamic_cast<Expression*>(result[0]) != nullptr);

        return (Expression*)result[0];
    }

    Statement* manuallyVisitStatement(CMParser::StatementContext *ctx)
    {
        antlrcpp::Any any = visitStatement(ctx);
        assert(any.isNotNull());

        Result result = any.as<Result>();
        assert(result.size() == 1);
        assert(dynamic_cast<Statement*>(result[0]) != nullptr);

        return (Statement*)result[0];
    }

    Result buildBinaryExpression(BinaryExpression::Operator op, antlr4::tree::ParseTree *tree)
    {
        Result result = visitChildren(tree).as<Result>();
        assert(result.size() == 2);
        assert(dynamic_cast<Expression*>(result[0]) != nullptr);
        assert(dynamic_cast<Expression*>(result[1]) != nullptr);

        return createResult(new BinaryExpression(op, (Expression*)result[0], (Expression*)result[1]));
    }

    std::vector<VarDeclaration> manuallyVisitIdentifierList(CMParser::IdentifierListContext *ctx)
    {
        std::vector<VarDeclaration> identifierList;
        for (auto funcParam: ctx->functionParameter()) {
            identifierList.push_back(manuallyVisitFunctionParameter(funcParam));
        }

        return identifierList;
    }

    VarDeclaration manuallyVisitFunctionParameter(CMParser::FunctionParameterContext *ctx)
    {
        const std::string typeName = ctx->typeIdentifier()->getText();
        const std::string varName = ctx->identifier()->getText();

        VarDeclaration decl;
        decl.type = TypeDecl::getFromString(typeName);
        decl.name = varName;
        return decl;
    }

public:
    Ast::Ptr convertTree(const AntlrContext *antlrContext)
    {
        antlr4::tree::ParseTree *tree = antlrContext->getParseTree();
        Ast *ast = visit(tree);
        return Ast::Ptr(ast);
    }

    virtual antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override
    {
        Result result;

        for (auto child: node->children) {
            antlrcpp::Any childResult = child->accept(this);
            if (childResult.isNotNull()) {
                Result childResultNodes = childResult.as<Result>();
                result.insert(result.end(), childResultNodes.begin(), childResultNodes.end());
            }
        }

        if (result.empty()) {
            return nullptr;
        }

        return result;
    }

    virtual antlrcpp::Any visitRoot(CMParser::RootContext *ctx) override
    {
        return visitRootBlock(ctx->rootBlock());
    }

    virtual antlrcpp::Any visitRootBlock(CMParser::RootBlockContext *ctx) override
    {
        Ast *ast = new Ast();

        for (CMParser::RootItemContext *rootItem: ctx->rootItem()) {
            CMParser::VariableDeclarationContext *varDecl = rootItem->variableDeclaration();
            CMParser::FunctionDefinitionContext *funcDef = rootItem->functionDefinition();
            CMParser::FunctionDeclarationContext *funcDecl = rootItem->functionDeclaration();

            // If this assert ever fails, the grammar has likely changed
            assert(varDecl || funcDef || funcDecl);

            if (varDecl != nullptr) {
                Result res = visitVariableDeclaration(varDecl).as<Result>();
                assert(res.size() == 1);
                assert(dynamic_cast<VariableDeclarationStatement*>(res[0]) != nullptr);
                ast->addRootStatement((VariableDeclarationStatement*)res[0]);
            } else if (funcDef != nullptr) {
                Result res = visitFunctionDefinition(funcDef).as<Result>();
                assert(res.size() == 1);
                assert(dynamic_cast<FunctionDefinition*>(res[0]) != nullptr);
                ast->addFunctionDefinition((FunctionDefinition*)res[0]);
            } else if (funcDecl != nullptr) {
                Result res = visitFunctionDeclaration(funcDecl);
                assert(res.size() == 1);
                assert(dynamic_cast<FunctionDeclarationStatement*>(res[0]) != nullptr);
                ast->addRootStatement((FunctionDeclarationStatement*)res[0]);
            }
        }

        return ast;
    }

    virtual antlrcpp::Any visitRootItem(CMParser::RootItemContext *ctx) override
    {
        // We parse the RootBlock's children explicitly, so this method should never get hit
        Throw(AstConversionException, "Internal conversion exception - should never visit RootItem");
    }

    virtual antlrcpp::Any visitExpression(CMParser::ExpressionContext *ctx) override
    {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitMULT_EXPR(CMParser::MULT_EXPRContext *ctx) override
    {
        BinaryExpression::Operator oper;
        if (ctx->op->getText() == "*") {
            oper = BinaryExpression::MULTIPLY;
        } else if (ctx->op->getText() == "/") {
            oper = BinaryExpression::DIVIDE;
        } else if (ctx->op->getText() == "%") {
            oper = BinaryExpression::MODULO;
        } else {
            Throw(AstConversionException, "Unable to handle operand '%s' as MULT_EXPR", ctx->op->getText().c_str());
        }

        return buildBinaryExpression(oper, ctx);
    }

    virtual antlrcpp::Any visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx) override
    {
        BinaryExpression::Operator oper;
        if (ctx->op->getText() == "==") {
            oper = BinaryExpression::EQ;
        } else if (ctx->op->getText() == "!=") {
            oper = BinaryExpression::NE;
        } else {
            Throw(AstConversionException, "Unable to handle operator '%s' as EQUALITY_EXPR", ctx->op->getText().c_str());
        }

        return buildBinaryExpression(oper, ctx);
    }

    virtual antlrcpp::Any visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx) override
    {
        const std::string literal = ctx->getText();
        return createResult(new LiteralExpression(literal));
    }

    virtual antlrcpp::Any visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx) override
    {
        return visitFunctionCall(ctx->functionCall());
    }

    virtual antlrcpp::Any visitADD_EXPR(CMParser::ADD_EXPRContext *ctx) override
    {
        BinaryExpression::Operator oper;
        if (ctx->op->getText() == "+") {
            oper = BinaryExpression::PLUS;
        } else if (ctx->op->getText() == "-") {
            oper = BinaryExpression::MINUS;
        } else {
            Throw(AstConversionException, "Unable to handle operator '%s' as ADD_EXPR", ctx->op->getText().c_str());
        }

        return buildBinaryExpression(oper, ctx);
    }

    virtual antlrcpp::Any visitAND_EXPR(CMParser::AND_EXPRContext *ctx) override
    {
        BinaryExpression::Operator oper;
        if (ctx->op->getText() == "&&") {
            oper = BinaryExpression::LOGICAL_AND;
        } else if (ctx->op->getText() == "||") {
            oper = BinaryExpression::LOGICAL_OR;
        } else {
            Throw(AstConversionException, "Unable to handle operator '%s' as AND_EXPR", ctx->op->getText().c_str());
        }

        return buildBinaryExpression(oper, ctx);
    }

    virtual antlrcpp::Any visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx) override
    {
        const std::string varName = ctx->Identifier()->getText();
        return createResult(new VariableReferenceExpression(&_declContext, varName));
    }

    virtual antlrcpp::Any visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx) override
    {
        BinaryExpression::Operator oper;
        if (ctx->op->getText() == ">=") {
            oper = BinaryExpression::GTE;
        } else if (ctx->op->getText() == ">") {
            oper = BinaryExpression::GT;
        } else if (ctx->op->getText() == "<=") {
            oper = BinaryExpression::LTE;
        } else if (ctx->op->getText() == "<") {
            oper = BinaryExpression::LT;
        } else {
            Throw(AstConversionException, "Unable to handle operator '%s' as COMPARE_EXPR", ctx->op->getText().c_str());
        }

        return buildBinaryExpression(oper, ctx);
    }

    virtual antlrcpp::Any visitStatement(CMParser::StatementContext *ctx) override
    {
        return visitChildren(ctx).as<Result>();
    }

    virtual antlrcpp::Any visitFunctionCallStatement(CMParser::FunctionCallStatementContext *ctx) override
    {
        Result res = visitChildren(ctx);
        assert(res.size() == 1);
        assert(dynamic_cast<FunctionCallExpression*>(res[0]) != nullptr);
        return createResult(new FunctionCallStatement((FunctionCallExpression*)res[0]));
    }

    virtual antlrcpp::Any visitReturnStatement(CMParser::ReturnStatementContext *ctx) override
    {
        Expression *expression = nullptr;
        if (ctx->expression()) {
            expression = manuallyVisitExpression(ctx->expression());
        }

        ReturnStatement *statement = new ReturnStatement(&_declContext, expression);
        return createResult(statement);
    }

    virtual antlrcpp::Any visitIfStatement(CMParser::IfStatementContext *ctx) override
    {
        Throw(AstNodeNotImplementedException, "Node of type 'IfStatement' is not yet supported as an AST-node!");
    }

    virtual antlrcpp::Any visitElseStatement(CMParser::ElseStatementContext *ctx) override
    {
        Throw(AstNodeNotImplementedException, "Node of type 'ElseStatement' is not yet supported as an AST-node!");
    }

    virtual antlrcpp::Any visitAssignment(CMParser::AssignmentContext *ctx) override
    {
        const std::string varName = ctx->identifier()->Identifier()->getText();

        Result result = visitChildren(ctx->expression());
        assert(result.size() == 1);
        assert(dynamic_cast<Expression*>(result[0]) != nullptr);
        Expression *expression = (Expression*)result[0];

        return createResult(new VariableAssignmentStatement(&_declContext, varName, expression));
    }

    virtual antlrcpp::Any visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) override
    {
        CMParser::TypeIdentifierContext *typeContext = ctx->typeIdentifier();

        if (typeContext->constTypeIdentifier() != nullptr) {
            Throw(AstConversionException, "Const variable declarations are not yet supported");
        }

        const std::string typeName = typeContext->mutableTypeIdentifier()->getText();
        const TypeDecl type = TypeDecl::getFromString(typeName);

        const std::string varName = ctx->identifier()->getText();

        CMParser::ExpressionContext *exprContext = ctx->expression();
        if (exprContext) {
            Expression *expr = manuallyVisitExpression(exprContext);
            return createResult(new VariableDeclarationStatement(&_declContext, type, varName, expr));
        } else {
            return createResult(new VariableDeclarationStatement(&_declContext, type, varName, nullptr));
        }
    }

    virtual antlrcpp::Any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) override
    {
        const std::string typeName = ctx->typeIdentifier()->getText();
        const std::string funcName = ctx->identifier()->getText();
        std::vector<VarDeclaration> params = manuallyVisitIdentifierList(ctx->identifierList());

        FuncDeclaration funcDecl;
        funcDecl.returnType = TypeDecl::getFromString(typeName);
        funcDecl.name = funcName;
        funcDecl.params = params;

        return createResult(new FunctionDeclarationStatement(&_declContext, funcDecl));
    }

    virtual antlrcpp::Any visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) override
    {
        const std::string typeName = ctx->typeIdentifier()->getText();
        const std::string funcName = ctx->identifier()->getText();
        std::vector<VarDeclaration> params = manuallyVisitIdentifierList(ctx->identifierList());

        FuncDeclaration funcDecl;
        funcDecl.returnType = TypeDecl::getFromString(typeName);
        funcDecl.name = funcName;
        funcDecl.params = params;

        // Potentially dirty - we need to perform all the logic related to the DeclarationContext
        // and variable declaration in a very specific order, and this is the best - if somewhat
        // awkward - place to do that.
        FunctionDefinition *funcDef = new FunctionDefinition(&_declContext, funcDecl);
        _declContext.enterFunction(funcDef);
        for (const VarDeclaration &varDecl: params) {
            _declContext.declareVariable(varDecl.type, varDecl.name);
        }

        std::vector<Statement*> statements;
        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement *statement = manuallyVisitStatement(stmtContext);
            funcDef->addStatement(statement);
        }

        _declContext.exitFunction();

        return createResult(funcDef);
    }

    virtual antlrcpp::Any visitFunctionCall(CMParser::FunctionCallContext *ctx) override
    {
        Result paramsResult = visitExpressionList(ctx->expressionList());
        const std::string funcName = ctx->identifier()->getText();

        std::vector<Expression*> params;
        for (AstNode *node: paramsResult) {
            assert(dynamic_cast<Expression*>(node) != nullptr);
            params.push_back((Expression*)node);
        }

        FunctionCallExpression *expr = new FunctionCallExpression(&_declContext, funcName, params);
        return createResult(expr);
    }

    virtual antlrcpp::Any visitExpressionList(CMParser::ExpressionListContext *ctx) override
    {
        Result result;
        for (CMParser::ExpressionContext *exprContext: ctx->expression()) {
            Expression *expr = manuallyVisitExpression(exprContext);
            result.push_back(expr);
        }

        return result;
    }

    virtual antlrcpp::Any visitIdentifierList(CMParser::IdentifierListContext *ctx) override
    {
        Throw(AstNodeNotImplementedException, "Node of type 'IdentifierList' is not yet supported as an AST-node!");
    }

    virtual antlrcpp::Any visitFunctionParameter(CMParser::FunctionParameterContext *ctx) override
    {
        // Will never be implemented! Use 'manuallyVisitFunctionParameter' instead!
        Throw(AstConversionException, "Internal conversion exception - should never visit FunctionParameter");
    }

    virtual antlrcpp::Any visitIdentifier(CMParser::IdentifierContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit Identifier");
    }

    virtual antlrcpp::Any visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit TypeIdentifier");
    }

    virtual antlrcpp::Any visitMutableTypeIdentifier(CMParser::MutableTypeIdentifierContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit MutableTypeIdentifier");
    }

    virtual antlrcpp::Any visitConstTypeIdentifier(CMParser::ConstTypeIdentifierContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit ConstTypeIdentifier");
    }
};

}



AstBuilder::AstBuilder(const AntlrContext *antlrContext):
    _antlrContext(antlrContext)
{

}

Ast::Ptr AstBuilder::buildAst()
{
    internal::TreeConverter converter;
    Ast::Ptr ast = converter.convertTree(_antlrContext);
    return std::move(ast);
}


}
}
