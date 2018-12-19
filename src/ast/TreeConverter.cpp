#include "TreeConverter.h"

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

namespace cish
{
namespace ast
{
namespace internal
{


TreeConverter::TreeConverter(ModuleContext::Ptr moduleContext):
    _moduleContext(std::move(moduleContext))
{
    _stringTable = StringTable::create();
}

Ast::Ptr TreeConverter::convertTree(const ParseContext *parseContext)
{
    _parseContext = parseContext;
    antlr4::tree::ParseTree *tree = _parseContext->getParseTree();
    Ast::Ptr ast = visit(tree).as<Ast::Ptr>();

    verifyAllFunctionsDefined(ast.get());
    ast->setStringTable(std::move(_stringTable));

    return ast;
}

antlrcpp::Any TreeConverter::visitChildren(antlr4::tree::ParseTree *node)
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

antlrcpp::Any TreeConverter::visitRoot(CMParser::RootContext *ctx)
{
    return visitRootBlock(ctx->rootBlock());
}

antlrcpp::Any TreeConverter::visitRootBlock(CMParser::RootBlockContext *ctx)
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
            std::string moduleName = visitSystemInclude(systemInclude).as<std::string>();
            includeModule(ast.get(), moduleName);
        }
    }

    return ast;
}

antlrcpp::Any TreeConverter::visitRootItem(CMParser::RootItemContext *ctx)
{
    // We parse the RootBlock's children explicitly, so this method should never get hit
    Throw(AstConversionException, "Internal conversion exception - should never visit RootItem");
}

antlrcpp::Any TreeConverter::visitSystemInclude(CMParser::SystemIncludeContext *ctx)
{
    std::string moduleName = ctx->sysModuleName()->getText();
    moduleName = moduleName.substr(1, moduleName.length() - 2);
    return moduleName;
}

antlrcpp::Any TreeConverter::visitExpression(CMParser::ExpressionContext *ctx)
{
    return visitChildren(ctx);
}

antlrcpp::Any TreeConverter::manuallyVisitIncdecexpr(CMParser::IncdecexprContext *ctx)
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

antlrcpp::Any TreeConverter::visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::POSTFIX_INCREMENT;
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
}

antlrcpp::Any TreeConverter::visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::PREFIX_INCREMENT;
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
}

antlrcpp::Any TreeConverter::visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::POSTFIX_DECREMENT;
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
}

antlrcpp::Any TreeConverter::visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::PREFIX_DECREMENT;
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<IncDecExpression>(&_declContext, op, varName));
}

antlrcpp::Any TreeConverter::visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx)
{
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<AddrofExpression>(&_declContext, varName));
}

antlrcpp::Any TreeConverter::visitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<DerefExpression>(&_declContext, expr));
}

antlrcpp::Any TreeConverter::visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<NegationExpression>(expr));
}

antlrcpp::Any TreeConverter::visitMULT_EXPR(CMParser::MULT_EXPRContext *ctx)
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

antlrcpp::Any TreeConverter::visitEQUALITY_EXPR(CMParser::EQUALITY_EXPRContext *ctx)
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

antlrcpp::Any TreeConverter::visitLITERAL_EXPR(CMParser::LITERAL_EXPRContext *ctx)
{
    const std::string literal = ctx->getText();
    return createResult(std::make_shared<LiteralExpression>(literal));
}

antlrcpp::Any TreeConverter::visitFUNC_CALL_EXPR(CMParser::FUNC_CALL_EXPRContext *ctx)
{
    return visitFunctionCall(ctx->functionCall());
}

antlrcpp::Any TreeConverter::visitADD_EXPR(CMParser::ADD_EXPRContext *ctx)
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

antlrcpp::Any TreeConverter::visitAND_EXPR(CMParser::AND_EXPRContext *ctx)
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

antlrcpp::Any TreeConverter::visitVAR_REF_EXPR(CMParser::VAR_REF_EXPRContext *ctx)
{
    const std::string varName = ctx->Identifier()->getText();
    return createResult(std::make_shared<VariableReferenceExpression>(&_declContext, varName));
}

antlrcpp::Any TreeConverter::visitCOMPARE_EXPR(CMParser::COMPARE_EXPRContext *ctx)
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

antlrcpp::Any TreeConverter::visitStatement(CMParser::StatementContext *ctx)
{
    return visitChildren(ctx).as<Result>();
}

antlrcpp::Any TreeConverter::visitReturnStatement(CMParser::ReturnStatementContext *ctx)
{
    Expression::Ptr expression = nullptr;
    if (ctx->expression()) {
        expression = manuallyVisitExpression(ctx->expression());
    }

    ReturnStatement::Ptr statement = std::make_shared<ReturnStatement>(&_declContext, expression);
    return createResult(statement);
}

antlrcpp::Any TreeConverter::visitIfStatement(CMParser::IfStatementContext *ctx)
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

antlrcpp::Any TreeConverter::visitElseStatement(CMParser::ElseStatementContext *ctx)
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

antlrcpp::Any TreeConverter::visitForStatement(CMParser::ForStatementContext *ctx)
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

antlrcpp::Any TreeConverter::visitWhileStatement(CMParser::WhileStatementContext *ctx)
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

antlrcpp::Any TreeConverter::visitDoWhileStatement(CMParser::DoWhileStatementContext *ctx)
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

antlrcpp::Any TreeConverter::visitExpressionStatement(CMParser::ExpressionStatementContext *ctx)
{
    Expression::Ptr expression = manuallyVisitExpression(ctx->expression());
    return createResult(std::make_shared<ExpressionStatement>(expression));
}

antlrcpp::Any TreeConverter::visitAssignment(CMParser::AssignmentContext *ctx)
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

antlrcpp::Any TreeConverter::visitVariableDeclaration(CMParser::VariableDeclarationContext *ctx)
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

antlrcpp::Any TreeConverter::visitFunctionDeclaration(CMParser::FunctionDeclarationContext *ctx)
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

antlrcpp::Any TreeConverter::visitFunctionDefinition(CMParser::FunctionDefinitionContext *ctx)
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

antlrcpp::Any TreeConverter::visitFunctionCall(CMParser::FunctionCallContext *ctx)
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

antlrcpp::Any TreeConverter::visitExpressionList(CMParser::ExpressionListContext *ctx)
{
    Result result;
    for (CMParser::ExpressionContext *exprContext: ctx->expression()) {
        Expression::Ptr expr = manuallyVisitExpression(exprContext);
        result.push_back(expr);
    }

    return result;
}

antlrcpp::Any TreeConverter::visitIdentifierList(CMParser::IdentifierListContext *ctx)
{
    Throw(AstNodeNotImplementedException, "Node of type 'IdentifierList' is not yet supported as an AST-node!");
}

antlrcpp::Any TreeConverter::visitFunctionParameter(CMParser::FunctionParameterContext *ctx)
{
    // Will never be implemented! Use 'manuallyVisitFunctionParameter' instead!
    Throw(AstConversionException, "Internal conversion exception - should never visit FunctionParameter");
}

antlrcpp::Any TreeConverter::visitStringLiteral(CMParser::StringLiteralContext *ctx)
{
    return createResult(manuallyVisitStringLiteral(ctx));
}

antlrcpp::Any TreeConverter::visitLvalue(CMParser::LvalueContext *ctx)
{
    // Will never be implemented!
    Throw(AstConversionException, "Internal conversion exception - should never visit Lvalue automatically!");
}

antlrcpp::Any TreeConverter::visitIdentifier(CMParser::IdentifierContext *ctx)
{
    // Will never be implemented!
    Throw(AstConversionException, "Internal conversion exception - should never visit Identifier");
}

antlrcpp::Any TreeConverter::visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx)
{
    std::vector<std::string> tokens;
    for (auto child: ctx->children) {
        tokens.push_back(child->getText());
    }
    TypeDecl type = TypeDecl::getFromTokens(tokens);
    return type;
}


/* PRIVATE */


TreeConverter::Result TreeConverter::createResult(AstNode *node)
{
    return Result { AstNode::Ptr(node) };
}

TreeConverter::Result TreeConverter::createResult(AstNode::Ptr node)
{
    return Result { node };
}

TreeConverter::Result TreeConverter::buildBinaryExpression(BinaryExpression::Operator op, antlr4::tree::ParseTree *tree)
{
    Result result = visitChildren(tree).as<Result>();
    assert(result.size() == 2);

    auto leftExpr = castToExpression(result[0]);
    auto rightExpr = castToExpression(result[1]);

    auto binaryExpr = std::make_shared<BinaryExpression>(op, leftExpr, rightExpr);
    return createResult(binaryExpr);
}

Expression::Ptr TreeConverter::castToExpression(AstNode::Ptr node)
{
    Expression::Ptr expr = std::dynamic_pointer_cast<Expression>(node);
    assert(expr != nullptr);
    return expr;
}

Statement::Ptr TreeConverter::castToStatement(AstNode::Ptr node)
{
    Statement::Ptr stmt = std::dynamic_pointer_cast<Statement>(node);
    assert(stmt != nullptr);
    return stmt;
}

Expression::Ptr TreeConverter::manuallyVisitExpression(CMParser::ExpressionContext *ctx)
{
    Result result = visitExpression(ctx).as<Result>();
    assert(result.size() == 1);
    return castToExpression(result[0]);
}

Statement::Ptr TreeConverter::manuallyVisitStatement(CMParser::StatementContext *ctx)
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

std::vector<VarDeclaration> TreeConverter::manuallyVisitIdentifierList(CMParser::IdentifierListContext *ctx)
{
    std::vector<VarDeclaration> identifierList;
    for (auto funcParam: ctx->functionParameter()) {
        identifierList.push_back(manuallyVisitFunctionParameter(funcParam));
    }

    return identifierList;
}

VarDeclaration TreeConverter::manuallyVisitFunctionParameter(CMParser::FunctionParameterContext *ctx)
{
    TypeDecl type = visitTypeIdentifier(ctx->typeIdentifier()).as<TypeDecl>();
    const std::string varName = ctx->identifier()->getText();

    VarDeclaration decl;
    decl.type = type;
    decl.name = varName;
    return decl;
}

Lvalue::Ptr TreeConverter::manuallyVisitLvalue(CMParser::LvalueContext *ctx)
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

StringLiteralExpression::Ptr TreeConverter::manuallyVisitStringLiteral(CMParser::StringLiteralContext *ctx)
{
    std::string str = ctx->getText();
    str = str.substr(1, str.length() - 2);
    str = ast::string::unescapeString(str);
    const StringId stringId = _stringTable->insert(str);
    return std::make_shared<StringLiteralExpression>(stringId);
}

Statement::Ptr TreeConverter::manuallyVisitForInitializer(CMParser::ForInitializerContext *ctx)
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

Statement::Ptr TreeConverter::manuallyVisitForIterator(CMParser::ForIteratorContext *ctx)
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

void TreeConverter::verifyAllFunctionsDefined(Ast *ast)
{
    for (FuncDeclaration decl: _funcDecls) {
        if (ast->getFunctionDefinition(decl.name) == nullptr) {
            Throw(FunctionNotDefinedException, "Function '%s' was declared but never defined",
                    decl.name.c_str());
        }
    }
}

void TreeConverter::includeModule(Ast *ast, std::string moduleName)
{
    if (_includedModules.count(moduleName) == 1) {
        return;
    }

    const module::Module::Ptr module = _moduleContext->getModule(moduleName);
    if (!module) {
        Throw(ModuleNotFoundException, "Could not include module '%s'", moduleName.c_str());
    }

    auto deps = module->getDependencies();
    for (const auto depName: deps) {
        includeModule(ast, depName);
    }

    auto functions = module->getFunctions();
    for (const auto &func: functions) {
        _declContext.declareFunction(*func->getDeclaration());
    }

    ast->addModule(module);
    _includedModules.insert(moduleName);
}

void TreeConverter::setSourcePosition(AstNode *astNode, antlr4::ParserRuleContext *ctx)
{
    const uint32_t start = ctx->getSourceInterval().a;
    const uint32_t end = ctx->getSourceInterval().b;

    SourcePosition pos = resolveSourcePosition(start, end);
    astNode->setSourcePosition(pos);
}

SourcePosition TreeConverter::resolveSourcePosition(uint32_t startChar, uint32_t endChar)
{
    SourcePosition pos;
    pos.startLine = _parseContext->getLineNumber(startChar);
    pos.globStartChar = startChar;
    pos.globEndChar = endChar;
    return pos;
}


}
}
}
