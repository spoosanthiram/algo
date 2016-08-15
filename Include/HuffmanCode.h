/**
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#ifndef PSA_HUFFMANCODE_H
#define PSA_HUFFMANCODE_H

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "MinHeap.h"

namespace psa {

struct HuffData
{
    HuffData(char sym) : symbol{sym} {}
    HuffData(char sym, unsigned int freq) : symbol{sym}, frequency{freq} {}
    char symbol;
    unsigned int frequency{0};
    std::string code{};
};

class HuffTree
{
public:
    struct Node
    {
        Node(const HuffData& x) : data{x} {}
        HuffData data;
        Node* left{nullptr};
        Node* right{nullptr};
    };

    using LessThan = bool(*)(HuffTree::Node* x, HuffTree::Node* y);

    static const int kMaxSymbol = 255;

public:
    HuffTree() = default;

    void build(std::istream& stream);
    void encode() { this->encode(m_minHeap->top(), std::string{}); }
    void printCodes(std::ostream& stream) { this->printCodes(m_minHeap->top(), stream); }

private:
    std::vector<HuffData> readSymbols(std::istream& stream);
    std::vector<Node*> buildNodes(const std::vector<HuffData>& symbols);
    void encode(Node* node, std::string code);
    void printCodes(Node* node, std::ostream& stream);

    MinHeap<Node*, LessThan, NullIndex<Node*>>* m_minHeap;
};

} // namespace psa

#endif // PSA_HUFFMANCODE_H
