#include "Trie.h"
#include <cassert>
#include <vector>

namespace cish::tok
{

Trie::Result Trie::search(std::string_view s) const
{
    const Node* node = &root;
    std::vector<Result> stack;

    int len = 0;
    for (const char &ch: s) {
        assert(ch >= 32 && ch <= 127);
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

void Trie::insert(std::string_view s, T type)
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
