#pragma once

#include <array>
#include <string_view>
#include <vector>

namespace cish::tok
{

template<typename T, uint32_t MinVal, uint32_t MaxVal>
class Trie 
{
public:
    struct Result
    {
        bool success{};
        int length{};
        std::optional<T> tokenType;
        bool operator==(const Result&) const = default;
    };

    Result search(std::string_view s) const;
    void insert(std::string_view s, T t);

private:
    struct Node
    {
        std::optional<T> value;
        std::array<std::unique_ptr<Node>, 95> children;

        Node* getOrCreate(char ch);
        const Node* get(char ch) const;
    };

    Node root;
};


template<typename T, uint32_t MinVal, uint32_t MaxVal>
Trie<T,MinVal,MaxVal>::Result Trie<T,MinVal,MaxVal>::search(std::string_view s) const
{
    const Node* node = &root;
    std::vector<Result> stack;

    int len = 0;
    for (const char &ch: s) {
        assert(ch >= MinVal && ch <= MaxVal);
        node = node->get(ch);
        len++;
        if (node == nullptr) {
            break;
        } else if (node->tokenType.has_value()) {
            stack.push_back({ true, len, node->tokenType.value() });
        }
    }

    if (stack.empty()) {
        return {};
    }
    return stack.back();
}

template<typename T>
void Trie<T>::insert(std::string_view s, T type)
{
    Node *node = &root;

    for (const char& c: s) {
        assert(c >= 32 && c<= 127);
        node = node->getOrCreate(c);
    }

    node->tokenType = type;
}

Trie::Node* Trie::Node::getOrCreate(char ch)
{
    const char idx = ch - 32;
    if (children[idx] == nullptr) {
        children[idx] = std::make_unique<Node>();
    }
    return children[idx].get();
}

const Trie::Node* Trie::Node::get(char ch) const
{
    const char idx = ch - 32;
    if (children[idx] == nullptr) {
        return nullptr;
    }
    return children[idx].get();
}


}


