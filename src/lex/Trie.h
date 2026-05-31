#pragma once

#include <array>
#include <vector>
#include <optional>
#include <span>
#include <cassert>
#include "TokenType.h"

namespace cish::lex
{

template<typename T, typename C, uint32_t MinVal, uint32_t MaxVal>
    requires std::is_convertible_v<C, uint32_t> 
          && (MaxVal > MinVal)
class Trie 
{
public:
    struct Result
    {
        T value;
        uint32_t length{};
        bool operator==(const Result&) const = default;
    };

    std::optional<Result> search(std::span<C> s) const
    {
        const Node* node = &root;
        std::vector<Result> stack;

        uint32_t len = 0;
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
                stack.push_back({ node->value.value(), len });
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
            const char idx = ch - MinVal;
            if (children[idx] == nullptr) {
                children[idx] = std::make_unique<Node>();
            }
            return children[idx].get();
        }

        const Node* get(char ch) const
        {
            const char idx = ch - MinVal;
            if (children[idx] == nullptr) {
                return nullptr;
            }
            return children[idx].get();
        }
    };

    Node root;
};

typedef Trie<TokenType, const char, 0, 127> TokenTrie;

}

