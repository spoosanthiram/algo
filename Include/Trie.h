/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_TRIE_H
#define PSA_TRIE_H

#include <array>
#include <string>
#include <vector>

namespace psa {

class Trie {
public:
    static const int kAlphabetSize = 26;
    static const int kNoValue = -1;

    struct Node {
        Node(int val = kNoValue) : value{val} { link.fill(nullptr); }
        int value;
        std::array<Node*, kAlphabetSize> link;
    };

public:
    Trie() { m_root = new Node{}; }

    int value(const std::string& key) const
    {
        Node* node = this->getNode(key);
        return node ? node->value : kNoValue;
    }
    std::vector<std::string> keysWithPrefix(const std::string& prefix) const
    {
        std::vector<std::string> result;
        this->keysWithPrefix(this->getNode(prefix), prefix, result);
        return result;
    }

    void insert(const std::string& key, int value);

private:
    Node* getNode(const std::string& key) const;
    void keysWithPrefix(Node* node, std::string key, std::vector<std::string>& result) const;

    Node* m_root;
};

} // namespace psa

#endif // PSA_TRIE_H
