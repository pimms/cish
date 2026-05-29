#pragma once

#include <optional>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace cish::tokvis {

struct FSEntry
{
    std::filesystem::directory_entry entry;
    std::vector<FSEntry> children;
    bool loaded = false;
};

class FileBrowser
{
public:
    FileBrowser();
    std::optional<std::string> render();

private:
    std::vector<FSEntry> _rootChildren;
    std::optional<std::string> _clickedFile;

    std::vector<FSEntry> loadDirectory(const std::string& path);
    void renderEntry(FSEntry& entry);
};

}
