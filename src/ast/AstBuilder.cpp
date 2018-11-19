#include "AstBuilder.h"

#include "antlr/CMBaseVisitor.h"
#include "AntlrContext.h"

#include "AstNodes.h"
#include "Lvalue.h"
#include "../ast/StringEscape.h"

#include "BinaryExpression.h"
#include "VariableReferenceExpression.h"
#include "FunctionCallExpression.h"
#include "LiteralExpression.h"
#include "IncDecExpression.h"
#include "AddrofExpression.h"
#include "DerefExpression.h"
#include "NegationExpression.h"
#include "StringLiteralExpression.h"

#include "VariableAssignmentStatement.h"
#include "VariableDeclarationStatement.h"
#include "FunctionDeclarationStatement.h"
#include "FunctionDefinition.h"
#include "ReturnStatement.h"
#include "IfStatement.h"
#include "ElseStatement.h"
#include "ForLoopStatement.h"
#include "WhileStatement.h"
#include "ExpressionStatement.h"

#include <cassert>


namespace cish
{
namespace ast
{

using module::Module;
using module::ModuleContext;

namespace internal
{

DECLARE_EXCEPTION(AstNodeNotImplementedException);
DECLARE_EXCEPTION(AstConversionException);

class TreeConverter: public CMBaseVisitor
{
    typedef std::vector<AstNode::Ptr> Result;
    DeclarationContext _declContext;
    StringTable::Ptr _stringTable;
    std::vector<FuncDeclaration> _funcDecls;
    const ModuleContext::Ptr _moduleContext;

    Result createResult(AstNode *node)
    {
        return Result { AstNode::Ptr(node) };
    }

    Result createResult(AstNode::Ptr node)
    {
        return Result { node };
    }

    Expression::Ptr castToExpression(AstNode::Ptr node)
    {
        Expression::Ptr expr = std::dynamic_pointer_cast<Expression>(node);
        assert(expr != nullptr);
        return expr;
    }

    Statement::Ptr castToStatement(AstNode::Ptr node)
    {
        Statement::Ptr stmt = std::dynamic_pointer_cast<Statement>(node);
        assert(stmt != nullptr);
        return stmt;
    }

    Expression::Ptr manuallyVisitExpression(CMParser::ExpressionContext *ctx)
    {
        Result result = visitExpression(ctx).as<Result>();
        assert(result.size() == 1);
        return castToExpression(result[0]);
    }

    Statement::Ptr manuallyVisitStatement(CMParser::StatementContext *ctx)
    {
        if (ctx->getText() == ";")
            return std::make_shared<NoOpStatement>();

        antlrcpp::Any any = visitStatement(ctx);
        assert(any.isNotNull());

        Result result = any.as<Result>();
        assert(result.size() == 1);

        Statement::Ptr statement = std::dynamic_pointer_cast<Statement>(result[0]);
        assert(statement != nullptr);
        return statement;
    }

    Result buildBinaryExpression(BinaryExpression::Operator op, antlr4::tree::ParseTree *tree)
    {
        Result result = visitChildren(tree).as<Result>();
        assert(result.size() == 2);

        auto leftExpr = castToExpression(result[0]);
        auto rightExpr = castToExpression(result[1]);

        auto binaryExpr = std::make_shared<BinaryExpression>(op, leftExpr, rightExpr);
        return createResult(binaryExpr);
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
        TypeDecl type = visitTypeIdentifier(ctx->typeIdentifier()).as<TypeDecl>();
        const std::string varName = ctx->identifier()->getText();

        VarDeclaration decl;
        decl.type = type;
        decl.name = varName;
        return decl;
    }

    void verifyAllFunctionsDefined(Ast *ast)
    {
        for (FuncDeclaration decl: _funcDecls) {
            if (ast->getFunctionDefinition(decl.name) == nullptr) {
                Throw(FunctionNotDefinedException, "Function '%s' was declared but never defined",
                        decl.name.c_str());
            }
        }
    }

public:
    TreeConverter(const ModuleContext::Ptr moduleContext):
        _moduleContext(moduleContext)
    {
        _stringTable = StringTable::create();
    }

    Ast::Ptr convertTree(const AntlrContext *antlrContext)
    {
        antlr4::tree::ParseTree *tree = antlrContext->getParseTree();
        Ast::Ptr ast = visit(tree).as<Ast::Ptr>();

        verifyAllFunctionsDefined(ast.get());
        ast->setStringTable(std::move(_stringTable));

        return ast;
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
        Ast::Ptr ast = std::make_shared<Ast>();

        for (CMParser::RootItemContext *rootItem: ctx->rootItem()) {
            CMParser::VariableDeclarationContext *varDecl = rootItem->variableDeclaration();
            CMParser::FunctionDefinitionContext *funcDef = rootItem->functionDefinition();
            CMParser::FunctionDeclarationContext *funcDecl = rootItem->functionDeclaration();
            CMParser::SystemIncludeContext *systemInclude = rootItem->systemInclude();

            // If this assert ever fails, the grammar has likely changed
            assert(varDecl || funcDef || funcDecl || systemInclude);

            if (varDecl != nullptr) {
                Result res = visitVariableDeclaration(varDecl).as<Result>();
                assert(res.size() == 1);
                ast->addRootStatement(castToStatement(res[0]));
            } else if (funcDef != nullptr) {
                Result res = visitFunctionDefinition(funcDef).as<Result>();
                assert(res.size() == 1);

                vm::Callable::Ptr callable = std::dynamic_pointer_cast<vm::Callable>(res[0]);
                assert(callable != nullptr);
                ast->addFunctionDefinition(callable);
            } else if (funcDecl != nullptr) {
                Result res = visitFunctionDeclaration(funcDecl);
                assert(res.size() == 1);
                assert(dynamic_cast<FunctionDeclarationStatement*>(res[0].get()) != nullptr);
                Statement::Ptr funcDeclStmt = castToStatement(res[0]);
                ast->addRootStatement(funcDeclStmt);
            } else if (systemInclude != nullptr) {
                Module::Ptr module = visitSystemInclude(systemInclude).as<Module::Ptr>();
                auto functions = module->getFunctions();
                for (const auto &func: functions) {
                    _declContext.declareFunction(*func->getDeclaration());
                }
                ast->addModule(module);
            }
        }

        return ast;
    }

    virtual antlrcpp::Any visitRootItem(CMParser::RootItemContext *ctx) override
    {
        // We parse the RootBlock's children explicitly, so this method should never get hit
        Throw(AstConversionException, "Internal conversion exception - should never visit RootItem");
    }

    virtual antlrcpp::Any visitSystemInclude(CMParser::SystemIncludeContext *ctx) override
    {
        std::string moduleName = ctx->sysModuleName()->getText();
        moduleName = moduleName.substr(1, moduleName.length() - 2);

        const module::Module::Ptr module = _moduleContext->getModule(moduleName);
        if (!module) {
            Throw(ModuleNotFoundException, "Could not include module '%s'", moduleName.c_str());
        }

        return module;
    }

    virtual antlrcpp::Any visitExpression(CMParser::ExpressionContext *ctx) override
    {
        return visitChildren(ctx);
    }


    virtual antlrcpp::Any manuallyVisitIncdecexpr(CMParser::IncdecexprContext *ctx)
    {
        if (dynamic_cast<CMParser::POSTFIX_INC_EXPRContext*>(ctx)) {
            return visitPOSTFIX_INC_EXPR((CMParser::POSTFIX_INC_EXPRContext*)ctx);
        } else if (dynamic_cast<CMParser::PREFIX_INC_EXPRContext*>(ctx)) {
            return visitPREFIX_INC_EXPR((CMParser::PREFIX_INC_EXPRContext*)ctx);
        } else if (dynamic_cast<CMParser::POSTFIX_DEC_EXPRContext*>(ctx)) {
            return visitPOSTFIX_DEC_EXPR((CMParser::POSTFIX_DEC_EXPRContext*)ctx);
        } else if (dynamic_cast<CMParser::PREFIX_DEC_EXPRContext*>(ctx)) {
            return visitPREFIX_DEC_EXPR((CMParser::PREFIX_DEC_EXPRContext*)ctx);
        } else {
            Throw(AstConversionException, "Manual visit of Incdecexpr failed");
        }
    }

    virtual antlrcpp::Any visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx) override
    {
        IncDecExpression::Operation op = IncDecExpression::POSTFIX_INCREMENT;
        const std::string varName = ctx->Identifier()->getText();
        return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
    }

    virtual antlrcpp::Any visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx) override
    {
        IncDecExpression::Operation op = IncDecExpression::PREFIX_INCREMENT;
        const std::string varName = ctx->Identifier()->getText();
        return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
    }

    virtual antlrcpp::Any visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx) override
    {
        IncDecExpression::Operation op = IncDecExpression::POSTFIX_DECREMENT;
        const std::string varName = ctx->Identifier()->getText();
        return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
    }

    virtual antlrcpp::Any visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx) override
    {
        IncDecExpression::Operation op = IncDecExpression::PREFIX_DECREMENT;
        const std::string varName = ctx->Identifier()->getText();
        return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
    }


    virtual antlrcpp::Any visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx) override
    {
        const std::string varName = ctx->Identifier()->getText();
        return createResult(std::make_shared<AddrofExpression>(&_declContext, varName));
    }

    virtual antlrcpp::Any visitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx) override
    {
        Result result = visitChildren(ctx).as<Result>();
        assert(result.size() == 1);
        Expression::Ptr expr = castToExpression(result[0]);
        return createResult(std::make_shared<DerefExpression>(&_declContext, expr));
    }

    virtual antlrcpp::Any visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx) override
    {
        Result result = visitChildren(ctx).as<Result>();
        assert(result.size() == 1);
        Expression::Ptr expr = castToExpression(result[0]);
        return createResult(std::make_shared<NegationExpression>(expr));
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
        return createResult(std::make_shared<LiteralExpression>(literal));
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
        return createResult(std::make_shared<VariableReferenceExpression>(&_declContext, varName));
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

    virtual antlrcpp::Any visitReturnStatement(CMParser::ReturnStatementContext *ctx) override
    {
        Expression::Ptr expression = nullptr;
        if (ctx->expression()) {
            expression = manuallyVisitExpression(ctx->expression());
        }

        ReturnStatement::Ptr statement = std::make_shared<ReturnStatement>(&_declContext, expression);
        return createResult(statement);
    }

    virtual antlrcpp::Any visitIfStatement(CMParser::IfStatementContext *ctx) override
    {
        ElseStatement::Ptr elseStatement = nullptr;
        if (ctx->elseStatement() != nullptr) {
            Result res = visitElseStatement(ctx->elseStatement()).as<Result>();
            assert(res.size() == 1);

            elseStatement = std::dynamic_pointer_cast<ElseStatement>(castToStatement(res[0]));
            assert(elseStatement != nullptr);
        }

        Expression::Ptr expr = manuallyVisitExpression(ctx->expression());
        IfStatement::Ptr ifStatement = std::make_shared<IfStatement>(expr, elseStatement);

        _declContext.pushVariableScope();

        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            ifStatement->addStatement(statement);
        }

        _declContext.popVariableScope();
        return createResult(std::dynamic_pointer_cast<AstNode>(ifStatement));
    }

    virtual antlrcpp::Any visitElseStatement(CMParser::ElseStatementContext *ctx) override
    {
        ElseStatement::Ptr elseStatement = std::make_shared<ElseStatement>();
        _declContext.pushVariableScope();

        std::vector<Statement*> statements;
        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            elseStatement->addStatement(statement);
        }

        _declContext.popVariableScope();
        return createResult(elseStatement);
    }

    virtual antlrcpp::Any visitForStatement(CMParser::ForStatementContext *ctx) override
    {
        Statement::Ptr initializer = nullptr;
        Expression::Ptr condition = nullptr;
        Statement::Ptr iterator = nullptr;

        _declContext.pushVariableScope();

        if (ctx->forInitializer())
            initializer = manuallyVisitForInitializer(ctx->forInitializer());
        if (ctx->expression())
            condition = manuallyVisitExpression(ctx->expression());
        if (ctx->forIterator())
            iterator = manuallyVisitForIterator(ctx->forIterator());

        ForLoopStatement::Ptr forLoop = std::make_shared<ForLoopStatement>(initializer, condition, iterator);

        std::vector<Statement*> statements;
        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            forLoop->addStatement(statement);
        }

        _declContext.popVariableScope();
        return createResult(forLoop);
    }

    Statement::Ptr manuallyVisitForInitializer(CMParser::ForInitializerContext *ctx)
    {
        Result res;
        if (ctx->assignment()) {
            res = visitAssignment(ctx->assignment()).as<Result>();
        } else if (ctx->variableDeclaration()) {
            res = visitVariableDeclaration(ctx->variableDeclaration()).as<Result>();
        } else {
            Throw(Exception, "Unsupported initialization in for-loop");
        }

        assert(res.size() == 1);
        return castToStatement(res[0]);
    }

    Statement::Ptr manuallyVisitForIterator(CMParser::ForIteratorContext *ctx)
    {
        Result res;
        if (ctx->assignment()) {
            res = visitAssignment(ctx->assignment()).as<Result>();
            assert(res.size() == 1);
            return castToStatement(res[0]);
        } else if (ctx->functionCall()) {
            res = visitFunctionCall(ctx->functionCall()).as<Result>();
            assert(res.size() == 1);
            auto functionCallExpression = castToExpression(res[0]);
            return std::make_shared<ExpressionStatement>(functionCallExpression);
        } else if (ctx->incdecexpr()) {
            res = manuallyVisitIncdecexpr(ctx->incdecexpr()).as<Result>();
            assert(res.size() == 1);
            auto incDecExpr = castToExpression(res[0]);
            return std::make_shared<ExpressionStatement>(incDecExpr);
        } else {
            Throw(Exception, "Unsupported initialization in for-loop");
        }
    }


    virtual antlrcpp::Any visitWhileStatement(CMParser::WhileStatementContext *ctx) override
    {
        Expression::Ptr condition = manuallyVisitExpression(ctx->expression());
        _declContext.pushVariableScope();

        WhileStatement::Ptr whileStatement = std::make_shared<WhileStatement>(condition);

        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            whileStatement->addStatement(statement);
        }

        _declContext.popVariableScope();
        return createResult(whileStatement);
    }

    virtual antlrcpp::Any visitDoWhileStatement(CMParser::DoWhileStatementContext *ctx) override
    {
        Expression::Ptr condition = manuallyVisitExpression(ctx->expression());
        _declContext.pushVariableScope();

        DoWhileStatement::Ptr doWhileStatement = std::make_shared<DoWhileStatement>(condition);

        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            doWhileStatement->addStatement(statement);
        }

        _declContext.popVariableScope();
        return createResult(doWhileStatement);
    }

    virtual antlrcpp::Any visitExpressionStatement(CMParser::ExpressionStatementContext *ctx) override
    {
        Expression::Ptr expression = manuallyVisitExpression(ctx->expression());
        return createResult(std::make_shared<ExpressionStatement>(expression));
    }


    virtual antlrcpp::Any visitAssignment(CMParser::AssignmentContext *ctx) override
    {
        Lvalue::Ptr lvalue = manuallyVisitLvalue(ctx->lvalue());
        Expression::Ptr expression = nullptr;

        if (ctx->expression()) {
            expression = manuallyVisitExpression(ctx->expression());
        } else {
            Throw(AstConversionException, "no rvalue for assignment");
        }

        auto varAssign = std::make_shared<VariableAssignmentStatement>(&_declContext, lvalue, expression);

        return createResult(varAssign);
    }

    virtual antlrcpp::Any visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx) override
    {
        CMParser::TypeIdentifierContext *typeContext = ctx->typeIdentifier();

        const TypeDecl type = visitTypeIdentifier(ctx->typeIdentifier()).as<TypeDecl>();;
        const std::string varName = ctx->identifier()->getText();

        Expression::Ptr expression = nullptr;
        if (ctx->expression()) {
            expression = manuallyVisitExpression(ctx->expression());
        }

        auto varDecl = std::make_shared<VariableDeclarationStatement>(&_declContext, type, varName, expression);

        return createResult(varDecl);
    }

    virtual antlrcpp::Any visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx) override
    {
        const std::string funcName = ctx->identifier()->getText();
        std::vector<VarDeclaration> params = manuallyVisitIdentifierList(ctx->identifierList());

        FuncDeclaration funcDecl;
        funcDecl.returnType = visitTypeIdentifier(ctx->typeIdentifier());
        funcDecl.name = funcName;
        funcDecl.params = params;

        _funcDecls.push_back(funcDecl);

        auto funcDeclStatement = std::make_shared<FunctionDeclarationStatement>(&_declContext, funcDecl);
        return createResult(funcDeclStatement);
    }

    virtual antlrcpp::Any visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx) override
    {
        const std::string funcName = ctx->identifier()->getText();
        std::vector<VarDeclaration> params = manuallyVisitIdentifierList(ctx->identifierList());

        FuncDeclaration funcDecl;
        funcDecl.returnType = visitTypeIdentifier(ctx->typeIdentifier());
        funcDecl.name = funcName;
        funcDecl.params = params;

        // Potentially dirty - we need to perform all the logic related to the DeclarationContext
        // and variable declaration in a very specific order, and this is the best - if somewhat
        // awkward - place to do that.
        FunctionDefinition::Ptr funcDef = std::make_shared<FunctionDefinition>(&_declContext, funcDecl);
        _declContext.enterFunction(funcDef);
        for (const VarDeclaration &varDecl: params) {
            _declContext.declareVariable(varDecl.type, varDecl.name);
        }

        for (CMParser::StatementContext *stmtContext: ctx->statement()) {
            Statement::Ptr statement = manuallyVisitStatement(stmtContext);
            funcDef->addStatement(statement);
        }

        _declContext.exitFunction();
        return createResult(funcDef);
    }

    virtual antlrcpp::Any visitFunctionCall(CMParser::FunctionCallContext *ctx) override
    {
        Result paramsResult = visitExpressionList(ctx->expressionList());
        const std::string funcName = ctx->identifier()->getText();

        std::vector<Expression::Ptr> params;
        for (AstNode::Ptr node: paramsResult) {
            Expression::Ptr expr = castToExpression(node);
            params.push_back(expr);
        }

        auto callExpr = std::make_shared<FunctionCallExpression>(&_declContext, funcName, params);
        return createResult(callExpr);
    }

    virtual antlrcpp::Any visitExpressionList(CMParser::ExpressionListContext *ctx) override
    {
        Result result;
        for (CMParser::ExpressionContext *exprContext: ctx->expression()) {
            Expression::Ptr expr = manuallyVisitExpression(exprContext);
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

    Lvalue::Ptr manuallyVisitLvalue(CMParser::LvalueContext *ctx)
    {
        std::string declName = ctx->getText();
        if (declName[0] != '*') {
            return std::make_shared<VariableReference>(&_declContext, declName);
        }

        int derefs = 0;
        while (declName[0] == '*') {
            derefs++;
            declName = declName.substr(1);
        }

        return std::make_shared<DereferencedVariableReference>(&_declContext, declName, derefs);
    }

    StringLiteralExpression::Ptr manuallyVisitStringLiteral(CMParser::StringLiteralContext *ctx)
    {
        std::string str = ctx->getText();
        str = str.substr(1, str.length() - 2);
        str = ast::string::unescapeString(str);
        const StringId stringId = _stringTable->insert(str);
        return std::make_shared<StringLiteralExpression>(stringId);
    }

    virtual antlrcpp::Any visitStringLiteral(CMParser::StringLiteralContext *ctx) override
    {
        return createResult(manuallyVisitStringLiteral(ctx));
    }

    virtual antlrcpp::Any visitLvalue(CMParser::LvalueContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit Lvalue automatically!");
    }

    virtual antlrcpp::Any visitIdentifier(CMParser::IdentifierContext *ctx) override
    {
        // Will never be implemented!
        Throw(AstConversionException, "Internal conversion exception - should never visit Identifier");
    }

    virtual antlrcpp::Any visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx) override
    {
        std::vector<std::string> tokens;
        for (auto child: ctx->children) {
            tokens.push_back(child->getText());
        }
        TypeDecl type = TypeDecl::getFromTokens(tokens);
        return type;
    }
};

}



AstBuilder::AstBuilder(const AntlrContext *antlrContext, const ModuleContext::Ptr moduleContext):
    _antlrContext(antlrContext),
    _moduleContext(moduleContext)
{

}

Ast::Ptr AstBuilder::buildAst()
{
    // TODO: Propagate these errors out of here in a more reasonable way
    if (_antlrContext->hasErrors()) {
        std::vector<ast::CompilationError> errors = _antlrContext->getErrors();

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

    internal::TreeConverter converter(_moduleContext);
    Ast::Ptr ast = converter.convertTree(_antlrContext);
    return std::move(ast);
}


}
}
