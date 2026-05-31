#include "FileBrowser.h"
#include <imgui.h>
#include <format>

namespace cish::tokvis
{

FileBrowser::FileBrowser()
    : _rootChildren(loadDirectory("."))
{

}

std::optional<std::string> FileBrowser::render()
{
    _clickedFile = std::nullopt;
    const auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y });
    ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->WorkPos.y });
    ImGui::Begin("filebrowser", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    if (ImGui::BeginTable("filetable", 1, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();
        for (auto& child: _rootChildren) {
            renderEntry(child);
        }
        ImGui::EndTable();
    }

    ImGui::End();
    return _clickedFile;
}

std::vector<FSEntry> FileBrowser::loadDirectory(const std::string &path)
{
    std::vector<FSEntry> children;
    for (auto& entry: std::filesystem::directory_iterator(path)) {
        children.emplace_back(entry, std::vector<FSEntry>(), !entry.is_directory());
    }

    return children;
}

void FileBrowser::renderEntry(FSEntry &entry)
{
    if (!entry.loaded) {
        entry.children = loadDirectory(entry.entry.path().string());
        entry.loaded = true;
    }

    const bool isDir = entry.entry.is_directory();
    int flags = ImGuiTreeNodeFlags_DrawLinesFull;
    if (!isDir) {
        flags |= ImGuiTreeNodeFlags_Leaf;
        flags |= ImGuiTreeNodeFlags_Bullet;
        flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    const std::string id = entry.entry.path().string();
    const std::string name = std::format(
        "{}{}",
        isDir ? "[DIR] " : "",
        entry.entry.path().filename().string());

    if (isDir) {
        if (ImGui::TreeNodeEx(id.c_str(), flags, "%s", name.c_str())) {
            for (auto& child: entry.children) {
                renderEntry(child);
            }
            ImGui::TreePop();
        }
    } else if (name.ends_with(".c")) {
        if (ImGui::Button(name.c_str())) {
            _clickedFile = entry.entry.path().string();
        }
    } else {
        ImGui::Text("%s", name.c_str());
    }
}

}
