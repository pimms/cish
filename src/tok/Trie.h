#pragma once

#include <array>
#include <vector>
#include <span>
#include <cassert>
#include "TokenType.h"

namespace cish::tok
{

template<typename T, typename C, uint32_t MinVal, uint32_t MaxVal>
    requires std::is_convertible_v<C, uint32_t> 
          && (MaxVal > MinVal)
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

    Result search(std::span<C> s) const
    {
        const Node* node = &root;
        std::vector<Result> stack;

        int len = 0;
        for (const auto &i: s) {
            if (std::is_same_v<const char, C> && static_cast<uint32_t>(i) == 0) {
                break;
            }

            assert(static_cast<uint32_t>(i) >= MinVal && static_cast<uint32_t>(i) <= MaxVal);
            node = node->get(i);
            len++;
            if (node == nullptr) {
                break;
            } else if (node->value.has_value()) {
                stack.push_back({ true, len, node->value.value() });
            }
        }

        if (stack.empty()) {
            return {};
        }
        return stack.back();
    }


    void insert(std::span<C> s, T t)
    {
        Node *node = &root;

        for (const C& i: s) {
            if (std::is_same_v<const char, C> && static_cast<uint32_t>(i) == 0) {
                break;
            }
            assert(static_cast<uint32_t>(i) >= MinVal && static_cast<uint32_t>(i) <= MaxVal);
            node = node->getOrCreate(i);
        }

        node->value = t;
    }

private:
    struct Node
    {
        std::optional<T> value;
        std::array<std::unique_ptr<Node>, MaxVal - MinVal> children;

        Node* getOrCreate(char ch)
        {
            const char idx = ch - 32;
            if (children[idx] == nullptr) {
                children[idx] = std::make_unique<Node>();
            }
            return children[idx].get();
        }

        const Node* get(char ch) const
        {
            const char idx = ch - 32;
            if (children[idx] == nullptr) {
                return nullptr;
            }
            return children[idx].get();
        }
    };

    Node root;
};

typedef Trie<TokenType, const char, 32, 127> TokenTrie;

}

