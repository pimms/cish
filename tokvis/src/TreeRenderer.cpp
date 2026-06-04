#include "TreeRenderer.h"
#include "lex/Lexer.h"
#include "parse/Parser.h"
#include "Visitor.h"

#include <utility>
#include <imgui.h>
#include <fstream>
#include <format>

namespace cish::tokvis {
namespace
{

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


void TreeRenderer::loadFile(const std::string &filepath)
{
    _tree = nullptr;
    _textContent = std::nullopt;

    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        _textContent = std::format("Failed to open file '{}'", filepath);
        return;
    }

    ifs.seekg(0, std::ios::end);
    const std::streamsize size = ifs.tellg();
    ifs.seekg(0);
    std::string sourceBuffer(size, '\0');
    ifs.read(&sourceBuffer[0], size);

    try {
        lex::Lexer tokenizer(sourceBuffer);
        auto tokens = tokenizer.tokenize();
        if (tokens.empty()) {
            _textContent = "File contains zero tokens. Let's assume this is an error in cish.";
            return;
        }

        parse::Parser parser(tokens);
        _textContent = sourceBuffer;
        _tree = parser.parse();
    } catch (const std::exception &e) {
        _textContent = std::format("Failed to parse '{}':\n{}\n", filepath, e.what());
    }
}

/*
================
DRAWING METHODS
================
*/

bool TreeRenderer::render()
{
    const auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y });
    ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->WorkPos.y });
    ImGui::Begin("Tokvis", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    bool close = ImGui::Button("CLOSE??");

    _idCounter = 0;


    // Render source on the left. We use previous-frame highlight (set by tree hovers
    // in the previous frame). This is simple, robust, and gives smooth hover feedback.
    if (_textContent.has_value()) {
        ImGui::BeginGroup();
        renderSourceText();
        ImGui::EndGroup();
        ImGui::SameLine();
    }

    _highlightedInterval = std::nullopt;
    renderTree();

    ImGui::End();
    return close;
}

void TreeRenderer::renderTree()
{
    if (_tree != nullptr) {
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
}

void TreeRenderer::renderSourceText()
{
    const std::string& text = _textContent.value();
    const bool hasHighlight = _highlightedInterval.has_value();
    const auto& hl = hasHighlight ? _highlightedInterval.value() : parse::CodeInterval{};

    // Use tight line spacing for source code
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    size_t pos = 0;
    const size_t textSize = text.size();

    while (pos < textSize) {
        // Find end of current line (excluding the newline itself)
        size_t lineEnd = text.find('\n', pos);
        if (lineEnd == std::string::npos) {
            lineEnd = textSize;
        }

        const char* lineBegin = text.data() + pos;
        const char* lineEndPtr = text.data() + lineEnd;

        const int lineStartOffset = static_cast<int>(pos);
        const int lineEndOffset = static_cast<int>(lineEnd);

        // Record cursor position before drawing this line (for highlight rects)
        const ImVec2 lineStartScreenPos = ImGui::GetCursorScreenPos();
        const float lineHeight = ImGui::GetTextLineHeight();

        // Draw background highlight for the portion of this line that overlaps the interval
        if (hasHighlight) {
            const int hlStart = hl.start.charOffset;
            const int hlEnd = hl.end.charOffset;

            const int overlapStart = std::max(lineStartOffset, hlStart);
            const int overlapEnd = std::min(lineEndOffset, hlEnd);

            if (overlapStart < overlapEnd) {
                // Measure width of text before the highlighted part on this line
                const float prefixWidth = (overlapStart > lineStartOffset)
                    ? ImGui::CalcTextSize(lineBegin, lineBegin + (overlapStart - lineStartOffset)).x
                    : 0.0f;

                // Measure width of the highlighted segment
                const float highlightWidth = ImGui::CalcTextSize(
                    lineBegin + (overlapStart - lineStartOffset),
                    lineBegin + (overlapEnd - lineStartOffset)
                ).x;

                const ImVec2 rectMin{ lineStartScreenPos.x + prefixWidth, lineStartScreenPos.y };
                const ImVec2 rectMax{ lineStartScreenPos.x + prefixWidth + highlightWidth,
                                      lineStartScreenPos.y + lineHeight };

                // Pleasant semi-transparent highlight (yellowish)
                ImGui::GetWindowDrawList()->AddRectFilled(
                    rectMin, rectMax,
                    IM_COL32(255, 220, 80, 65)
                );
            }
        }

        // Draw the line itself (handles empty lines gracefully)
        if (lineBegin != lineEndPtr) {
            ImGui::TextUnformatted(lineBegin, lineEndPtr);
        } else {
            // Empty line — still need to advance the cursor
            ImGui::Dummy(ImVec2(1.0f, lineHeight));
        }

        // Advance to next line
        pos = (lineEnd < textSize) ? (lineEnd + 1) : lineEnd;
    }

    ImGui::PopStyleVar();
}

bool TreeRenderer::renderNode(const parse::CodeInterval& interval, const std::string &name, const std::string &value, bool hasChildren)
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

    // Whole-row hover detection using the recommended Selectable + SpanAllColumns pattern
    const std::string rowSelId = std::format("##row{}", _idCounter);
    if (ImGui::Selectable(rowSelId.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)) {
    }
    if (ImGui::IsItemHovered()) {
        _highlightedInterval = interval;
    }

    ImGui::SameLine(0.0f, 0.0f);
    const bool result = ImGui::TreeNodeEx(nameWithId.c_str(), flags);
    ImGui::TableNextColumn();
    ImGui::Text("%s", value.c_str());

    return hasChildren && result;
}

void TreeRenderer::renderLeafNode(const parse::CodeInterval& interval, const std::string& name, const std::string& value)
{
    renderNode(interval, name, value, false);
}

bool TreeRenderer::renderParentNode(const parse::CodeInterval& interval, const std::string& name, const std::string& value)
{
    return renderNode(interval, name, value, true);
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

    if (renderNode(node.interval, "Variable declaration", value, node.expression != nullptr)) {
        renderExpression(*node.expression);
        endParentNode();
    }
}

void TreeRenderer::renderFunctionDeclaration(const parse::FunctionDeclaration &node)
{
    renderLeafNode(node.interval, "Function declaration", toString(node));
}

void TreeRenderer::renderFunctionDefinition(const parse::FunctionDefinition &node)
{
    if (renderParentNode(node.interval, "Function def", toString(node.declaration))) {
        for (const auto& s: node.body) {
            renderStatement(*s);
        }

        endParentNode();
    }
}

void TreeRenderer::renderSystemInclude(const parse::SystemInclude &node)
{
    renderLeafNode(node.interval, "System include", std::format("#include <{}>", node.moduleName));
}

void TreeRenderer::renderStructDeclaration(const parse::StructDeclaration &node)
{
    if (renderParentNode(node.interval, "Struct declaration", node.name)) {
        for (const auto& field: node.fields) {
            renderLeafNode(field.interval, field.type.toString(), field.name);
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
    if (renderParentNode(node.interval, "If statement", "")) {
        renderExpression(*node.condition);

        if (renderParentNode(node.interval, "true-body", "")) {
            renderStatement(*node.positiveBody);
            endParentNode();
        }
        if (node.negativeBody != nullptr && renderParentNode(node.interval, "false-body", "")) {
            renderStatement(*node.negativeBody);
            endParentNode();
        }

        endParentNode();
    }
}

void TreeRenderer::renderReturnStatement(const parse::ReturnStatement &node)
{
    if (renderNode(node.interval, "Return statement", "", node.expression != nullptr)) {
        renderExpression(*node.expression);
        endParentNode();
    }
}

void TreeRenderer::renderForStatement(const parse::ForStatement &node)
{
    if (renderParentNode(node.interval, "For loop", "")) {
        if (renderNode(node.interval, "init", node.initializer == nullptr ? "nil" : "", node.initializer != nullptr)) {
            std::visit(Visitor{
                [this](const std::unique_ptr<parse::IExpression>& e) { renderExpression(*e); },
                [this](const std::unique_ptr<parse::IStatement>& s) { renderStatement(*s); },
            }, *node.initializer);
            endParentNode();
        }
        if (renderNode(node.interval, "condition", node.condition == nullptr ? "nil" : "", node.initializer != nullptr)) {
            renderExpression(*node.condition);
            endParentNode();
        }
        if (renderNode(node.interval, "update", node.update == nullptr ? "nil" : "", node.update != nullptr)) {
            renderExpression(*node.update);
            endParentNode();
        }

        renderStatement(*node.body);
        endParentNode();
    }
}

void TreeRenderer::renderWhileStatement(const parse::WhileStatement &node)
{
    if (renderParentNode(node.interval, "While statement", "")) {
        renderExpression(*node.condition);
        renderStatement(*node.body);
        endParentNode();
    }
}

void TreeRenderer::renderDoWhileStatement(const parse::DoWhileStatement &node)
{
    if (renderParentNode(node.interval, "Do-while statement", "")) {
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
    if (renderParentNode(node.interval, "Scope", "")) {
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
        [this](const parse::MemberAccessExpr& v) { renderMemberAccessExpression(v); },
        [this](const parse::SizeofExpr& v) { renderSizeofExpression(v); }
        }, node
    );
}

void TreeRenderer::renderSubscriptExpression(const parse::SubscriptExpr &node)
{
    if (renderParentNode(node.interval, "Subscript expression", "")) {
        renderExpression(*node.ptrExpression);
        renderExpression(*node.idxExpression);
        endParentNode();
    }
}

void TreeRenderer::renderFunctionCallExpression(const parse::FunctionCallExpr &node)
{
    if (renderNode(node.interval, "Function call", node.functionName, !node.params.empty())) {
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

    if (renderParentNode(node.interval, "Binary expr", oper)) {
        renderExpression(*node.left);
        renderExpression(*node.right);
        endParentNode();
    }
}

void TreeRenderer::renderTypeCastExpression(const parse::TypeCastExpr &node)
{
    if (renderParentNode(node.interval, "Type cast", node.type.toString())) {
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

    if (renderParentNode(node.interval, "Unary expr", oper)) {
        renderExpression(*node.expr);
        endParentNode();
    }
}

void TreeRenderer::renderVarRefExpression(const parse::VarRefExpr &node)
{
    renderLeafNode(node.interval, "Variable ref", node.identifier);
}

void TreeRenderer::renderBoolLiteralExpression(const parse::BoolLiteralExpr &node)
{
    renderLeafNode(node.interval, "Bool literal", std::format("{}", node.value));
}

void TreeRenderer::renderCharLiteralExpression(const parse::CharLiteralExpr &node)
{
    renderLeafNode(
        node.interval,
       "Char literal",
        std::format("{:c} ({:d})", (char)node.value, node.value)
    );
}

void TreeRenderer::renderIntLiteralExpr(const parse::IntLiteralExpr &node)
{
    renderLeafNode(node.interval, "Int literal", std::format("{:d}", node.value));
}

void TreeRenderer::renderFloatLiteralExpression(const parse::FloatLiteralExpr &node)
{
    renderLeafNode(node.interval, "Float literal", std::format("{:g}", node.value));
}

void TreeRenderer::renderStringLIteralExpression(const parse::StringLiteralExpr &node)
{
    renderLeafNode(node.interval, "String literal", std::format("\"{}\"", node.value));
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
        renderLeafNode(node.interval, "Member access", value);
    } else {
        std::string value = std::format("{}{}", oper, node.member);
        if (renderParentNode(node.interval, "Member access", node.member)) {
            renderExpression(*node.expr);
            endParentNode();
        }
    }
}

void TreeRenderer::renderSizeofExpression(const parse::SizeofExpr &node)
{
    std::visit(Visitor {
        [this, &node](const std::unique_ptr<parse::IExpression>& e) {
            if (renderParentNode(node.interval, "sizeof", "")) {
                renderExpression(*e);
                endParentNode();
            }
        },
        [this, &node](const parse::TypeIdentifier& t) {
            renderLeafNode(node.interval, "sizeof", t.toString());
        }
    }, node.term);
}

}
