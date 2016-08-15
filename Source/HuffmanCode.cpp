/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "HuffmanCode.h"

#include <algorithm>

#ifdef UNIT_TEST
#include <fstream>

#include <gtest/gtest.h>

#include "AlgoException.h"
#endif // UNIT_TEST

namespace psa {

void HuffTree::build(std::istream& stream)
{
    std::vector<HuffData> symbols = this->readSymbols(stream);

    LessThan ltLambda = [](HuffTree::Node* x, HuffTree::Node* y) -> bool
        { return x->data.frequency < y->data.frequency; };

    m_minHeap = new MinHeap<Node*, LessThan, NullIndex<Node*>>{
        this->buildNodes(symbols), ltLambda,  NullIndex<Node*>{}};

    while (m_minHeap->size() > 1) {
        Node* rightNode = m_minHeap->pop();
        Node* leftNode = m_minHeap->pop();

        Node* node = new Node{HuffData{0, leftNode->data.frequency + rightNode->data.frequency}};
        node->left = leftNode;
        node->right = rightNode;

        m_minHeap->push(node);
    }
}

std::vector<HuffData> HuffTree::readSymbols(std::istream& stream)
{
    std::vector<HuffData> symbols;

    symbols.reserve(kMaxSymbol + 1);

    for (int i = 0; i <= kMaxSymbol; ++i)
        symbols.emplace_back(i);

    char ch;
    while (stream.get(ch)) {
        if (ch < 0 || static_cast<int>(ch) >= kMaxSymbol)
            throw AlgoException{"Character not supported currently!"};

        ++(symbols[ch].frequency);
    }

    auto it = std::remove_if(symbols.begin(), symbols.end(),
                             [](const HuffData& x) { return x.frequency == 0; });
    symbols.erase(it, symbols.end());

    return symbols;
}

std::vector<HuffTree::Node*> HuffTree::buildNodes(const std::vector<HuffData>& symbols)
{
    std::vector<Node*> nodes;

    for (auto& data : symbols) {
        Node* node = new Node{data};
        nodes.push_back(node);
    }

    return nodes;
}

void HuffTree::encode(Node* node, std::string code)
{
    if (!node)
        return;

    node->data.code = code;

    this->encode(node->left, code + "0");
    this->encode(node->right, code + "1");
}

unsigned long g_totalBitsNeeded = 0;

void HuffTree::printCodes(Node* node, std::ostream& stream)
{
    if (!node)
        return;

    std::string symbol;
    if (node->data.symbol != 0) {
        if (node->data.symbol == ' ')
            symbol.assign("sp");
        else if (node->data.symbol == '\n')
            symbol.assign("nl");
        else if (node->data.symbol == '\t')
            symbol.assign("tab");
        else
            symbol = node->data.symbol;
        stream << symbol << " --> " << node->data.code << "\n";

        g_totalBitsNeeded += node->data.code.size() * node->data.frequency;
    }

    this->printCodes(node->left, stream);
    this->printCodes(node->right, stream);
}

#ifdef UNIT_TEST

TEST(HuffmanCodeTest, HuffmanCode)
{
    const std::string filename{"HuffmanInput.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    HuffTree ht;
    ht.build(stream);
    ht.encode();
    ht.printCodes(std::cout);

    std::cout << "Total Bits: " << g_totalBitsNeeded << std::endl;
}

#endif // UNIT_TEST

} // namespace psa
