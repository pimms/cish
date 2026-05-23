#pragma once

#include "parse/ParseTree.h"

#include <format>
#include <utility>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>


namespace cish::tokvis {

class TreeRenderer final
{
public:
    explicit TreeRenderer(std::unique_ptr<parse::ParseTree> tree);
    ~TreeRenderer() = default;
    TreeRenderer() = delete;
    TreeRenderer(const TreeRenderer&) = delete;
    TreeRenderer(TreeRenderer&&) = delete;
    TreeRenderer& operator=(const TreeRenderer&) = delete;
    TreeRenderer& operator=(TreeRenderer&&) = delete;

    void render();

private:
    std::unique_ptr<parse::ParseTree> _tree;
    int _idCounter = 0;

    bool renderNode(const std::string& name, const std::string& value, bool hasChildren);
    void renderLeafNode(const std::string& name, const std::string& value);
    bool renderParentNode(const std::string& name, const std::string& value);
    void endParentNode();

    void renderRootItem(const parse::IRootItem& rootItem);
    void renderVariableDeclaration(const parse::VariableDeclarationStatement& node);
    void renderFunctionDeclaration(const parse::FunctionDeclaration& node);
    void renderFunctionDefinition(const parse::FunctionDefinition& node);
    void renderSystemInclude(const parse::SystemInclude& node);
    void renderStructDeclaration(const parse::StructDeclaration& node);

    void renderStatement(const parse::IStatement& node);
    void renderIfStatement(const parse::IfStatement& node);
    void renderReturnStatement(const parse::ReturnStatement& node);
    void renderForStatement(const parse::ForStatement& node);
    void renderWhileStatement(const parse::WhileStatement& node);
    void renderDoWhileStatement(const parse::DoWhileStatement& node);
    void renderExpressionStatement(const parse::ExpressionStatement& node);
    void renderScopeStatement(const parse::ScopeStatement& node);

    void renderExpression(const parse::IExpression& node);
    void renderSubscriptExpression(const parse::SubscriptExpr& node);
    void renderFunctionCallExpression(const parse::FunctionCallExpr& node);
    void renderBinaryExpression(const parse::BinaryExpr& node);
    void renderTypeCastExpression(const parse::TypeCastExpr& node);
    void renderUnaryExpression(const parse::UnaryExpr& node);
    void renderVarRefExpression(const parse::VarRefExpr& node);
    void renderBoolLiteralExpression(const parse::BoolLiteralExpr& node);
    void renderCharLiteralExpression(const parse::CharLiteralExpr& node);
    void renderIntLiteralExpr(const parse::IntLiteralExpr& node);
    void renderFloatLiteralExpression(const parse::FloatLiteralExpr& node);
    void renderStringLIteralExpression(const parse::StringLiteralExpr& node);
    void renderMemberAccessExpression(const parse::MemberAccessExpr& node);
};

};