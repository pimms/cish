#pragma once

#include "TokenType.h"
#include <array>
#include <string_view>

namespace cish::tok
{

class Trie 
{
public:
    struct Result
    {
        bool success{};
        int length{};
        std::optional<TokenType> tokenType;
        bool operator==(const Result&) const = default;
    };

    Result search(std::string_view s) const;
    void insert(std::string_view s, TokenType type);

private:
    struct Node
    {
        std::optional<TokenType> tokenType;
        std::array<std::unique_ptr<Node>, 95> children;

        Node* getOrCreate(char ch);
        const Node* get(char ch) const;
    };

    Node root;
};

}


