/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_BINARYSEACHTREE_H
#define PSA_BINARYSEACHTREE_H

#include "BinaryTree.h"

namespace psa {

template<typename NodeType>
class BinarySearchTree : public BinaryTree<NodeType>
{
public:
    BinarySearchTree() : BinaryTree<NodeType>{} {}
    BinarySearchTree(const std::vector<int>& numbers);
    BinarySearchTree(const BinarySearchTree& rhs) = delete; // TODO:
    BinarySearchTree(BinarySearchTree&& rhs)
    {
        BinaryTree<NodeType>::m_root = rhs.m_root;
        rhs.m_root = nullptr;
    }
    ~BinarySearchTree() {}

    BinarySearchTree operator=(const BinarySearchTree& rhs) = delete; // TODO:
    BinarySearchTree operator=(BinarySearchTree&& rhs) = delete; // TODO:

    void insert(int value) override;
    void buildBalanced(const std::vector<int>& sorted);

protected:
    void buildBalanced(const int* p, const int* r, BinaryTreeNode<NodeType>** node);
};

template<typename NodeType>
BinarySearchTree<NodeType>::BinarySearchTree(const std::vector<int>& numbers)
    : BinaryTree<NodeType>{}
{
    for (auto i : numbers)
        this->insert(i);
}

template<typename NodeType>
void BinarySearchTree<NodeType>::insert(int value)
{
    BinaryTreeNode<NodeType>** node = &(BinaryTree<NodeType>::m_root);
    while (*node) {
        if (value < (*node)->value)
            node = &(*node)->left;
        else
            node = &(*node)->right;
    }
    *node = new BinaryTreeNode<NodeType>{value};
}

template<typename NodeType>
void BinarySearchTree<NodeType>::buildBalanced(const std::vector<int>& sorted)
{
    const int* p = sorted.data();
    const int* r = &p[sorted.size() - 1];
    this->buildBalanced(p, r, &BinaryTree<NodeType>::m_root);
}

template<typename NodeType>
void BinarySearchTree<NodeType>::buildBalanced(const int* p, const int* r,
                                               BinaryTreeNode<NodeType>** node)
{
    if (p > r)
        return;

    const int* q = p + (r - p) / 2;
    *node = new BinaryTreeNode<NodeType>{*q};
    this->buildBalanced(p, q - 1, &(*node)->left);
    this->buildBalanced(q + 1, r, &(*node)->right);
}

} // namespace psa

#endif // PSA_BINARYSEACHTREE_H
