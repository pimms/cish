#include "TreeRenderer.h"

#include <format>

namespace cish::tokvis {

namespace
{
    // Thanks to https://andreasfertig.com/blog/2023/07/visiting-a-stdvariant-safely/
    template<class... Ts> struct Visitor: Ts...
    {
        using Ts::operator()...;
        consteval void operator()(auto) const { static_assert(false, "Unsupported type"); }
    };
    template<class... Ts>
    Visitor(Ts...) -> Visitor<Ts...>;

    std::string toString(const parse::FunctionDeclaration& decl)
    {
        std::string value = std::format("{} {}(",
            decl.returnType.toString(),
            decl.name);

        int nparams = 0;

        for (const auto& param: decl.params) {
            value += std::format("{}{}{}{}",
                nparams > 0 ? ", " : "",
                param.type.toString(),
                param.name.has_value() ? " " : "",
                param.name.value_or("")
            );

            nparams++;
        }
        value += ")";
        return value;
    }
}

TreeRenderer::TreeRenderer(std::unique_ptr<parse::ParseTree> tree)
    : _tree(std::move(tree))
{

}

/*
================
DRAWING METHODS
================
*/

void TreeRenderer::render()
{
    _idCounter = 0;
    if (ImGui::BeginTable("table", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody)) {
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        for (const auto& rootItem: _tree->rootItems) {
            renderRootItem(rootItem);
        }
        ImGui::EndTable();
    }
}

bool TreeRenderer::renderNode(const std::string &name, const std::string &value, bool hasChildren)
{
    const std::string nameWithId = std::format("{}##{}", name, _idCounter++);

    int flags = ImGuiTreeNodeFlags_DrawLinesFull;
    if (!hasChildren) {
        flags |= ImGuiTreeNodeFlags_Leaf;
        flags |= ImGuiTreeNodeFlags_Bullet;
        flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    bool result = ImGui::TreeNodeEx(nameWithId.c_str(), flags);
    ImGui::TableNextColumn();
    ImGui::Text("%s", value.c_str());

    return hasChildren && result;
}

void TreeRenderer::renderLeafNode(const std::string& name, const std::string& value)
{
    renderNode(name, value, false);
}

bool TreeRenderer::renderParentNode(const std::string& name, const std::string& value)
{
    return renderNode(name, value, true);
}

void TreeRenderer::endParentNode()
{
    ImGui::TreePop();
}

/*
================
ROOT ITEMS
================
*/

void TreeRenderer::renderRootItem(const parse::IRootItem &rootItem)
{
    std::visit(Visitor {
        [this](const parse::VariableDeclarationStatement& v) { renderVariableDeclaration(v); },
        [this](const parse::FunctionDeclaration& v) { renderFunctionDeclaration(v); },
        [this](const parse::FunctionDefinition& v) { renderFunctionDefinition(v); },
        [this](const parse::SystemInclude& v) { renderSystemInclude(v); },
        [this](const parse::StructDeclaration& v) { renderStructDeclaration(v); }
    }, rootItem);
}

void TreeRenderer::renderVariableDeclaration(const parse::VariableDeclarationStatement &node)
{
    std::string value = std::format("{} {}", node.type.toString(), node.varName);

    if (renderNode("Variable declaration", value, node.expression != nullptr)) {
        renderExpression(*node.expression);
        endParentNode();
    }
}

void TreeRenderer::renderFunctionDeclaration(const parse::FunctionDeclaration &node)
{
    renderLeafNode("Function declaration", toString(node));
}

void TreeRenderer::renderFunctionDefinition(const parse::FunctionDefinition &node)
{
    if (renderParentNode("Function def", toString(node.declaration))) {
        for (const auto& s: node.body) {
            renderStatement(*s);
        }

        endParentNode();
    }
}

void TreeRenderer::renderSystemInclude(const parse::SystemInclude &node)
{
    renderLeafNode("System include", std::format("#include <{}>", node.moduleName));
}

void TreeRenderer::renderStructDeclaration(const parse::StructDeclaration &node)
{
    if (renderParentNode("Struct declaration", node.name)) {
        for (const auto& field: node.fields) {
            renderLeafNode(field.type.toString(), field.name);
        }
        endParentNode();
    }
}

/*
================
STATEMENT
================
*/

void TreeRenderer::renderStatement(const parse::IStatement &node)
{
    std::visit(Visitor {
        [this](const parse::IfStatement& v) { renderIfStatement(v); },
        [this](const parse::VariableDeclarationStatement& v) { renderVariableDeclaration(v); },
        [this](const parse::ReturnStatement& v) { renderReturnStatement(v); },
        [this](const parse::ForStatement& v) { renderForStatement(v); },
        [this](const parse::WhileStatement& v) { renderWhileStatement(v); },
        [this](const parse::DoWhileStatement& v) { renderDoWhileStatement(v); },
        [this](const parse::ExpressionStatement& v) { renderExpressionStatement(v); },
        [this](const parse::ScopeStatement& v) { renderScopeStatement(v); }
        }, node
    );
}

void TreeRenderer::renderIfStatement(const parse::IfStatement &node)
{
    if (renderParentNode("If statement", "")) {
        renderExpression(*node.condition);

        if (renderParentNode("true-body", "")) {
            renderStatement(*node.positiveBody);
            endParentNode();
        }
        if (node.negativeBody != nullptr && renderParentNode("false-body", "")) {
            renderStatement(*node.negativeBody);
            endParentNode();
        }

        endParentNode();
    }
}

void TreeRenderer::renderReturnStatement(const parse::ReturnStatement &node)
{
    if (renderNode("Return statement", "", node.expression != nullptr)) {
        renderExpression(*node.expression);
        endParentNode();
    }
}

void TreeRenderer::renderForStatement(const parse::ForStatement &node)
{
    if (renderParentNode("For loop", "")) {
        if (renderNode("init", node.initializer == nullptr ? "nil" : "", node.initializer != nullptr)) {
            std::visit(Visitor{
                [this](const std::unique_ptr<parse::IExpression>& e) { renderExpression(*e); },
                [this](const std::unique_ptr<parse::IStatement>& s) { renderStatement(*s); },
            }, *node.initializer);
            endParentNode();
        }
        if (renderNode("condition", node.condition == nullptr ? "nil" : "", node.initializer != nullptr)) {
            renderExpression(*node.condition);
            endParentNode();
        }
        if (renderNode("update", node.update == nullptr ? "nil" : "", node.update != nullptr)) {
            renderExpression(*node.update);
            endParentNode();
        }

        renderStatement(*node.body);
        endParentNode();
    }
}

void TreeRenderer::renderWhileStatement(const parse::WhileStatement &node)
{
    if (renderParentNode("While statement", "")) {
        renderExpression(*node.condition);
        renderStatement(*node.body);
        endParentNode();
    }
}

void TreeRenderer::renderDoWhileStatement(const parse::DoWhileStatement &node)
{
    if (renderParentNode("Do-while statement", "")) {
        renderExpression(*node.condition);
        renderStatement(*node.body);
        endParentNode();
    }
}

void TreeRenderer::renderExpressionStatement(const parse::ExpressionStatement &node)
{
    renderExpression(*node.expression);
}

void TreeRenderer::renderScopeStatement(const parse::ScopeStatement &node)
{
    if (renderParentNode("Scope", "")) {
        for (const auto& s: node.body) {
            renderStatement(*s);
        }
        endParentNode();
    }
}

/*
================
EXPRESSIONS
================
*/

void TreeRenderer::renderExpression(const parse::IExpression &node)
{
    std::visit(Visitor {
    [this](const parse::SubscriptExpr& v) { renderSubscriptExpression(v); },
    [this](const parse::FunctionCallExpr& v) { renderFunctionCallExpression(v); },
    [this](const parse::BinaryExpr& v) { renderBinaryExpression(v); },
    [this](const parse::TypeCastExpr& v) { renderTypeCastExpression(v); },
    [this](const parse::UnaryExpr& v) { renderUnaryExpression(v); },
    [this](const parse::VarRefExpr& v) { renderVarRefExpression(v); },
    [this](const parse::BoolLiteralExpr& v) { renderBoolLiteralExpression(v); },
    [this](const parse::CharLiteralExpr& v) { renderCharLiteralExpression(v); },
    [this](const parse::IntLiteralExpr& v) { renderIntLiteralExpr(v); },
    [this](const parse::FloatLiteralExpr& v) { renderFloatLiteralExpression(v); },
    [this](const parse::StringLiteralExpr& v) { renderStringLIteralExpression(v); },
    [this](const parse::MemberAccessExpr& v) { renderMemberAccessExpression(v); }
        }, node
    );
}

void TreeRenderer::renderSubscriptExpression(const parse::SubscriptExpr &node)
{
    if (renderParentNode("Subscript expression", "")) {
        renderExpression(*node.ptrExpression);
        renderExpression(*node.idxExpression);
        endParentNode();
    }
}

void TreeRenderer::renderFunctionCallExpression(const parse::FunctionCallExpr &node)
{
    if (renderNode("Function call", node.functionName, !node.params.empty())) {
        for (const auto& param: node.params) {
            renderExpression(*param);
        }
        endParentNode();
    }
}

void TreeRenderer::renderBinaryExpression(const parse::BinaryExpr &node)
{
    std::string oper;
    switch (node.oper) {
        case parse::BinaryOperator::MULT: oper = "*"; break;
        case parse::BinaryOperator::DIVIDE: oper ="/"; break;
        case parse::BinaryOperator::MODULO: oper = "%"; break;
        case parse::BinaryOperator::PLUS: oper = "+"; break;
        case parse::BinaryOperator::MINUS: oper = "-"; break;
        case parse::BinaryOperator::LSHIFT: oper = "<<"; break;
        case parse::BinaryOperator::RSHIFT: oper = ">>"; break;
        case parse::BinaryOperator::GTE: oper = ">="; break;
        case parse::BinaryOperator::LTE: oper = "<="; break;
        case parse::BinaryOperator::GT: oper = ">"; break;
        case parse::BinaryOperator::LT: oper = "<"; break;
        case parse::BinaryOperator::EQUALS: oper = "=="; break;
        case parse::BinaryOperator::NEQUALS: oper = "!="; break;
        case parse::BinaryOperator::BITAND: oper = "&"; break;
        case parse::BinaryOperator::BITXOR: oper = "^"; break;
        case parse::BinaryOperator::BITOR: oper = "|"; break;
        case parse::BinaryOperator::LOGAND: oper = "&&"; break;
        case parse::BinaryOperator::LOGOR: oper = "||"; break;
        case parse::BinaryOperator::ASSIGN: oper = "="; break;
        case parse::BinaryOperator::ASS_MULT: oper = "*="; break;
        case parse::BinaryOperator::ASS_DIVIDE: oper = "/="; break;
        case parse::BinaryOperator::ASS_MODULO: oper = "%="; break;
        case parse::BinaryOperator::ASS_PLUS: oper = "+="; break;
        case parse::BinaryOperator::ASS_MINUS: oper = "-="; break;
        case parse::BinaryOperator::ASS_LSHIFT: oper = "<<="; break;
        case parse::BinaryOperator::ASS_RSHIFT: oper = ">>="; break;
        case parse::BinaryOperator::ASS_BITAND: oper = "&="; break;
        case parse::BinaryOperator::ASS_BITXOR: oper = "^="; break;
        case parse::BinaryOperator::ASS_BITOR: oper = "|="; break;
    }

    if (renderParentNode("Binary expr", oper)) {
        renderExpression(*node.left);
        renderExpression(*node.right);
        endParentNode();
    }
}

void TreeRenderer::renderTypeCastExpression(const parse::TypeCastExpr &node)
{
    if (renderParentNode("Type cast", node.type.toString())) {
        renderExpression(*node.expr);
        endParentNode();
    }
}

void TreeRenderer::renderUnaryExpression(const parse::UnaryExpr &node)
{
    std::string oper;
    switch (node.oper) {
        case parse::UnaryOperator::POSTINC: oper = "n++"; break;
        case parse::UnaryOperator::POSTDEC: oper = "n--"; break;
        case parse::UnaryOperator::PREINC: oper = "++n"; break;
        case parse::UnaryOperator::PREDEC: oper = "--n"; break;
        case parse::UnaryOperator::MINUS: oper = "-n"; break;
        case parse::UnaryOperator::NEGATE: oper = "!n"; break;
        case parse::UnaryOperator::ONES_COMPL: oper = "~n"; break;
        case parse::UnaryOperator::DEREF: oper = "*n"; break;
        case parse::UnaryOperator::ADDROF: oper = "&n"; break;
        case parse::UnaryOperator::SIZEOF: oper = "sizeof n"; break;
    }

    if (renderParentNode("Unary expr", oper)) {
        renderExpression(*node.expr);
        endParentNode();
    }
}

void TreeRenderer::renderVarRefExpression(const parse::VarRefExpr &node)
{
    renderLeafNode("Variable ref", node.identifier);
}

void TreeRenderer::renderBoolLiteralExpression(const parse::BoolLiteralExpr &node)
{
    renderLeafNode("Bool literal", std::format("{}", node.value));
}

void TreeRenderer::renderCharLiteralExpression(const parse::CharLiteralExpr &node)
{
    renderLeafNode(
       "Char literal",
        std::format("{:c} ({:d})", (char)node.value, node.value)
    );
}

void TreeRenderer::renderIntLiteralExpr(const parse::IntLiteralExpr &node)
{
    renderLeafNode("Int literal", std::format("{:d}", node.value));
}

void TreeRenderer::renderFloatLiteralExpression(const parse::FloatLiteralExpr &node)
{
    renderLeafNode("Float literal", std::format("{:g}", node.value));
}

void TreeRenderer::renderStringLIteralExpression(const parse::StringLiteralExpr &node)
{
    renderLeafNode("String literal", std::format("\"{}\"", node.value));
}

void TreeRenderer::renderMemberAccessExpression(const parse::MemberAccessExpr &node)
{
    std::string oper;
    switch (node.oper) {
        case parse::MemberAccessOperator::DOT:
            oper = ".";
            break;
        case parse::MemberAccessOperator::ARROW:
            oper = "->";
            break;
    }

    if (std::holds_alternative<parse::VarRefExpr>(*node.expr)) {
        const auto &expr = std::get<parse::VarRefExpr>(*node.expr);
        std::string value = std::format("{}{}{}", expr.identifier, oper, node.member);
        renderLeafNode("Member access", value);
    } else {
        std::string value = std::format("{}{}", oper, node.member);
        if (renderParentNode("Member access", node.member)) {
            renderExpression(*node.expr);
            endParentNode();
        }
    }
}

}
