#include "TreeConverter.h"

#include "BinaryExpression.h"
#include "FunctionCallExpression.h"
#include "LiteralExpression.h"
#include "IncDecExpression.h"
#include "AddrofExpression.h"
#include "NegationExpression.h"
#include "OnesComplementExpression.h"
#include "StringLiteralExpression.h"
#include "SizeofExpression.h"
#include "TypeCastExpression.h"
#include "ArithmeticAssignmentStatement.h"
#include "MinusExpression.h"
#include "StructAccessExpression.h"

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

#include "StructLayout.h"
#include "StructField.h"


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
    antlr4::tree::ParseTree *tree = parseContext->getParseTree();
    Ast::Ptr ast = visit(tree).as<Ast::Ptr>();

    verifyAllFunctionsDefined(ast.get());
    ast->setStringTable(std::move(_stringTable));

    return ast;
}

antlrcpp::Any TreeConverter::visitChildren(antlr4::tree::ParseTree *node)
{
    Result result;

    for (auto child: node->children) {
        if (dynamic_cast<CMParser::IdentifierContext*>(child) != nullptr) {
            continue;
        }

        antlrcpp::Any childResult = child->accept(this);
        if (childResult.isNotNull() && childResult.is<Result>()) {
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
        CMParser::StructDeclarationContext *structDecl = rootItem->structDeclaration();

        // If this assert ever fails, the grammar has likely changed
        assert(varDecl || funcDef || funcDecl || systemInclude || structDecl);

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
        } else if (structDecl != nullptr) {
            auto structLayout = visitStructDeclaration(structDecl).as<StructLayout::Ptr>();
            ast->addStructLayout(structLayout);
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

antlrcpp::Any TreeConverter::visitSTRUCT_ACCESS_EXPR(CMParser::STRUCT_ACCESS_EXPRContext *ctx)
{
    Result res = visitChildren(ctx);
    assert(res.size() == 1);
    Expression::Ptr expr = castToExpression(res[0]);

    const std::string method = ctx->op->getText();
    StructAccessExpression::AccessType accessMethod;
    if (method == ".") {
        accessMethod = StructAccessExpression::AccessType::OBJECT;
    } else if (method == "->") {
        accessMethod = StructAccessExpression::AccessType::POINTER;
    } else {
        Throw(AstConversionException, "'%s' is not a valid struct member access operator", method.c_str());
    }

    const std::string identifier = ctx->identifier()->getText();

    return createResult(std::make_shared<StructAccessExpression>(expr, identifier, accessMethod));
}

antlrcpp::Any TreeConverter::visitPOSTFIX_INC_EXPR(CMParser::POSTFIX_INC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::POSTFIX_INCREMENT;
    Result res = visitChildren(ctx).as<Result>();
    assert(res.size() == 1);
    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    return createResult(std::make_shared<IncDecExpression>(op, lvalue));
}

antlrcpp::Any TreeConverter::visitPREFIX_INC_EXPR(CMParser::PREFIX_INC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::PREFIX_INCREMENT;
    Result res = visitChildren(ctx).as<Result>();
    assert(res.size() == 1);
    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    return createResult(std::make_shared<IncDecExpression>(op, lvalue));
}

antlrcpp::Any TreeConverter::visitPOSTFIX_DEC_EXPR(CMParser::POSTFIX_DEC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::POSTFIX_DECREMENT;
    Result res = visitChildren(ctx).as<Result>();
    assert(res.size() == 1);
    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    return createResult(std::make_shared<IncDecExpression>(op, lvalue));
}

antlrcpp::Any TreeConverter::visitPREFIX_DEC_EXPR(CMParser::PREFIX_DEC_EXPRContext *ctx)
{
    IncDecExpression::Operation op = IncDecExpression::PREFIX_DECREMENT;
    Result res = visitChildren(ctx).as<Result>();
    assert(res.size() == 1);
    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    return createResult(std::make_shared<IncDecExpression>(op, lvalue));
}

antlrcpp::Any TreeConverter::visitTYPE_CAST_EXPR(CMParser::TYPE_CAST_EXPRContext *ctx)
{
    TypeDecl type = visitTypeIdentifier(ctx->typeIdentifier()).as<TypeDecl>();

    Result exprResult = visitChildren(ctx).as<Result>();
    assert(exprResult.size() == 1);
    Expression::Ptr expression = castToExpression(exprResult[0]);

    auto castExpr = std::make_shared<TypeCastExpression>(type, expression);
    return createResult(castExpr);
}

antlrcpp::Any TreeConverter::visitDEREF_EXPR(CMParser::DEREF_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<DereferenceExpression>(expr));
}

antlrcpp::Any TreeConverter::visitADDROF_EXPR(CMParser::ADDROF_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Lvalue::Ptr lvalue = castToLvalue(result[0]);
    return createResult(std::make_shared<AddrofExpression>(lvalue));
}

antlrcpp::Any TreeConverter::visitSIZEOF_EXPR(CMParser::SIZEOF_EXPRContext *ctx)
{
    SizeofExpression::Ptr sizeofExpr = nullptr;
    antlrcpp::Any evaluated = visitSizeofTerm(ctx->sizeofTerm());
    if (evaluated.is<Result>()) {
        Result result = evaluated.as<Result>();
        assert(result.size() == 1);
        Expression::Ptr expr = castToExpression(result[0]);
        sizeofExpr = std::make_shared<SizeofExpression>(expr);
    } else if (evaluated.is<TypeDecl>()) {
        sizeofExpr = std::make_shared<SizeofExpression>(evaluated.as<TypeDecl>());
    } else {
        Throw(AstConversionException, "Unexpected type in sizeofTerm");
    }

    return createResult(sizeofExpr);
}

antlrcpp::Any TreeConverter::visitSizeofTerm(CMParser::SizeofTermContext *ctx)
{
    if (ctx->sizeofTerm()) {
        return visitSizeofTerm(ctx->sizeofTerm());
    } else if (ctx->typeIdentifier()) {
        TypeDecl type = visitTypeIdentifier(ctx->typeIdentifier()).as<TypeDecl>();
        return type;
    } else {
        antlrcpp::Any evaluated = visitChildren(ctx);
        Result result = evaluated.as<Result>();
        assert(result.size() == 1);
        Expression::Ptr expr = castToExpression(result[0]);
        return createResult(expr);
    }
}

antlrcpp::Any TreeConverter::visitMINUS_EXPR(CMParser::MINUS_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<MinusExpression>(expr));
}

antlrcpp::Any TreeConverter::visitNEGATION_EXPR(CMParser::NEGATION_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<NegationExpression>(expr));
}

antlrcpp::Any TreeConverter::visitSUBSCRIPT_EXPR(CMParser::SUBSCRIPT_EXPRContext *ctx)
{
    Result res = visitChildren(ctx);
    assert(res.size() == 2);

    Expression::Ptr ptrExpr = castToExpression(res[0]);
    Expression::Ptr idxExpr = castToExpression(res[1]);

    auto subscript = std::make_shared<SubscriptExpression>(ptrExpr, idxExpr);
    return createResult(subscript);
}

antlrcpp::Any TreeConverter::visitONES_COMPLEMENT_EXPR(CMParser::ONES_COMPLEMENT_EXPRContext *ctx)
{
    Result result = visitChildren(ctx).as<Result>();
    assert(result.size() == 1);
    Expression::Ptr expr = castToExpression(result[0]);
    return createResult(std::make_shared<OnesComplementExpression>(expr));
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

antlrcpp::Any TreeConverter::visitBITSHIFT_EXPR(CMParser::BITSHIFT_EXPRContext *ctx)
{
    BinaryExpression::Operator oper;
    if (ctx->op->getText() == "<<") {
        oper = BinaryExpression::BITWISE_LSHIFT;
    } else if (ctx->op->getText() == ">>") {
        oper = BinaryExpression::BITWISE_RSHIFT;
    } else {
        Throw(AstConversionException, "Unable to handle operator '%s' as BITSHIFT_EXPR", ctx->op->getText().c_str());
    }

    return buildBinaryExpression(oper, ctx);
}

antlrcpp::Any TreeConverter::visitBITWISE_EXPR(CMParser::BITWISE_EXPRContext *ctx)
{
    BinaryExpression::Operator oper;
    if (ctx->op->getText() == "|") {
        oper = BinaryExpression::BITWISE_OR;
    } else if (ctx->op->getText() == "&") {
        oper = BinaryExpression::BITWISE_AND;
    } else if (ctx->op->getText() == "^") {
        oper = BinaryExpression::BITWISE_XOR;
    } else {
        Throw(AstConversionException, "Unable to handle operator '%s' as BITWISE_EXPR", ctx->op->getText().c_str());
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
    return createResult(std::make_shared<VariableReference>(&_declContext, varName));
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
    Result res = visitChildren(ctx).as<Result>();
    if (res.size() != 2) {
        Throw(AstConversionException, "Expected 2 expressions in assignment, found %d", (int)res.size());
    }

    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    Expression::Ptr rvalue = castToExpression(res[1]);

    auto varAssign = std::make_shared<VariableAssignmentStatement>(&_declContext, lvalue, rvalue);
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

antlrcpp::Any TreeConverter::visitStructDeclaration(CMParser::StructDeclarationContext *ctx)
{
    const std::string name = ctx->identifier()->getText();

    StructLayout *rawStruct = new StructLayout(name);
    StructLayout::Ptr sharedStruct = StructLayout::Ptr(rawStruct);
    _declContext.declareStruct(rawStruct);

    std::vector<std::pair<TypeDecl,std::string>> fields;
    for (auto fieldDecl: ctx->structFieldDeclaration()) {
        const TypeDecl type = visitTypeIdentifier(fieldDecl->typeIdentifier()).as<TypeDecl>();;
        const std::string varName = fieldDecl->identifier()->getText();
        fields.push_back(std::make_pair(type, varName));
    }

    if (fields.empty()) {
        Throw(EmptyStructException, "Struct '%s' has no declared fields", name.c_str());
    }

    for (const auto &field: fields) {
        rawStruct->addField(new StructField(field.first, field.second));
    }

    rawStruct->finalize();

    return sharedStruct;
}

antlrcpp::Any TreeConverter::visitArithmeticAssignment(CMParser::ArithmeticAssignmentContext *ctx)
{
    static const std::map<std::string, BinaryExpression::Operator> opmap = {
        { "+=",     BinaryExpression::Operator::PLUS }, 
        { "-=",     BinaryExpression::Operator::MINUS },
        { "*=",     BinaryExpression::Operator::MULTIPLY },
        { "/=",     BinaryExpression::Operator::DIVIDE },
        { "%=",     BinaryExpression::Operator::MODULO },
        { "<<=",    BinaryExpression::Operator::BITWISE_LSHIFT },
        { ">>=",    BinaryExpression::Operator::BITWISE_RSHIFT },
        { "&=",     BinaryExpression::Operator::BITWISE_AND },
        { "^=",     BinaryExpression::Operator::BITWISE_XOR },
        { "|=",     BinaryExpression::Operator::BITWISE_OR },
    };

    Result res = visitChildren(ctx).as<Result>();
    if (res.size() != 2) {
        Throw(AstConversionException, "Expected 2 expressions in arithmetic assignment, found %d", (int)res.size());
    }

    Lvalue::Ptr lvalue = castToLvalue(res[0]);
    Expression::Ptr rvalue = castToExpression(res[1]);

    const std::string op = ctx->op->getText();
    if (opmap.count(op) == 0)
        Throw(AstConversionException, "Unable to handle arith.ass. operator '%s'", op.c_str());
    BinaryExpression::Operator oper = opmap.at(op);

    auto stmt = std::make_shared<ArithmeticAssignmentStatement>(lvalue, oper, rvalue);
    return createResult(stmt);
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
        if (varDecl.name != "") {
            _declContext.declareVariable(varDecl.type, varDecl.name);
        }
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

antlrcpp::Any TreeConverter::visitIdentifier(CMParser::IdentifierContext *ctx)
{
    // Will never be implemented!
    Throw(AstConversionException, "Internal conversion exception - should never visit Identifier");
}

antlrcpp::Any TreeConverter::visitTypeIdentifier(CMParser::TypeIdentifierContext *ctx)
{
    std::vector<std::string> tokens = tokenizeTree(ctx);
    TypeDecl type = TypeDecl::getFromTokens(&_declContext, tokens);
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
    if (expr == nullptr) {
        Throw(AstConversionException, "Expected expression");
    }

    return expr;
}

Lvalue::Ptr TreeConverter::castToLvalue(AstNode::Ptr node)
{
    Lvalue::Ptr lvalue = std::dynamic_pointer_cast<Lvalue>(node);
    if (lvalue == nullptr) {
        // TODO: This will produce some absolute bullshit "compiler" errors,
        // as we strip away any line information or other context.
        Throw(AstConversionException, "Expected lvalue");
    }

    return lvalue;
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
    const std::string varName = (ctx->identifier() ? ctx->identifier()->getText() : "");

    VarDeclaration decl;
    decl.type = type;
    decl.name = varName;
    return decl;
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
    } else if (ctx->arithmeticAssignment()) {
        res = visitArithmeticAssignment(ctx->arithmeticAssignment()).as<Result>();
        assert(res.size() == 1);
        return castToStatement(res[0]);
    } else if (ctx->expression()) {
        Expression::Ptr expr = manuallyVisitExpression(ctx->expression());
        return std::make_shared<ExpressionStatement>(expr);
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


static void treeTokenizerHelper(std::vector<std::string> &res, antlr4::tree::ParseTree *tree);

std::vector<std::string> TreeConverter::tokenizeTree(antlr4::tree::ParseTree *tree)
{
    std::vector<std::string> res;
    treeTokenizerHelper(res, tree);
    return res;
}

static void treeTokenizerHelper(std::vector<std::string> &res, antlr4::tree::ParseTree *tree)
{
    if (tree->children.empty()) {
        res.push_back(tree->getText());
    } else {
        for (auto child: tree->children) {
            treeTokenizerHelper(res, child);
        }
    }
}



}
}
}
