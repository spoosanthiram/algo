/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Trie.h"

#include <iostream>

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa  {

void Trie::insert(const std::string& key, int value)
{
    // TODO: CHECK value != kNoValue
    Node* node = m_root;
    for (char ch : key) {
        ch = ch - 'a';
        if (ch < 0 || ch >= kAlphabetSize)
            ; // TODO: throw an exception

        if (node->link[ch])
            node = node->link[ch];
        else {
            node->link[ch] = new Node{};
            node = node->link[ch];
        }
    }
    node->value = value;
}

Trie::Node* Trie::getNode(const std::string& key) const
{
    Node* node = m_root;
    char ch;
    for (std::size_t i = 0; node && i < key.size(); ++i) {
        ch = key[i] - 'a';
        if (ch < 0 || ch >= kAlphabetSize)
            ; // TODO: throw an exception
        node = node->link[ch];
    }
    return node;
}

void Trie::keysWithPrefix(Node* node, std::string key, std::vector<std::string>& result) const
{
    if (!node)
        return;

    if (node->value != kNoValue)
        result.push_back(key);

    for (char i = 0; i < kAlphabetSize; ++i) {
        if (node->link[i]) {
            std::string newKey = key;
            newKey += ('a' + i);
            keysWithPrefix(node->link[i], newKey, result);
        }
    }
}

#ifdef UNIT_TEST

TEST(TrieTest, Simple)
{
    Trie trie;
    trie.insert("she", 0);
    trie.insert("sells", 1);
    trie.insert("sea", 2);
    trie.insert("shells", 3);
    trie.insert("by", 4);
    trie.insert("the", 5);
    trie.insert("shore", 6);

    EXPECT_EQ(4, trie.value("by"));

    std::vector<std::string> expected{};
    expected.push_back("she");
    expected.push_back("shells");
    expected.push_back("shore");
    auto actual = trie.keysWithPrefix("sh");

    EXPECT_EQ(expected, actual);
}

#endif // UNIT_TEST

} // namespace psa
