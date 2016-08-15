/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_BINARYTREE_H
#define PSA_BINARYTREE_H

#include <cstdlib>
#include <functional>
#include <stack>
#include <vector>

namespace psa {

template<typename NodeType>
struct BinaryTreeNode {
    BinaryTreeNode(const NodeType& val) : value{val} {}
    NodeType value;
    BinaryTreeNode<NodeType>* left{nullptr};
    BinaryTreeNode<NodeType>* right{nullptr};
};

template<typename NodeType>
class BinaryTree {
public:
    virtual ~BinaryTree()
    {
        // TODO: delete all nodes
    }

    const BinaryTreeNode<NodeType>* root() const { return m_root; }

    int height() const { return this->height(m_root); }
    std::vector<int> inorder() const
    {
        std::vector<int> elements{};
        this->inorder(m_root, elements);
        return elements;
    }
    std::vector<int> inorderNonRecursive() const;
    std::vector<int> preorder() const
    {
        std::vector<int> elements{};
        this->preorder(m_root, elements);
        return elements;
    }
    bool isBalanced() const
    {
        if (!m_root)
            return true;

        return std::abs(this->heightPlus1(m_root->left) - this->heightPlus1(m_root->right)) <= 1;
    }
    bool isSymmetric() const {
        if (!m_root)
            return true;
        return this->isSymmetric(m_root->left, m_root->right);
    }
    std::pair<std::size_t, unsigned int> maxInLevel(const std::function<bool(int)>& pred) const;
    std::pair<bool, int> leastCommonAncestor(int a, int b) const
    {
        BinaryTreeNode<NodeType>* lcaNode = this->leastCommonAncestor(m_root, a, b);
        if (lcaNode)
            return std::pair<bool, int>{true, lcaNode->value};
        else
            return std::pair<bool, int>{false, 0};
    }

    virtual void insert(int value) = 0;

protected:
    int height(const BinaryTreeNode<NodeType>* node) const;
    void inorder(BinaryTreeNode<NodeType>* node, std::vector<int>& elements) const;
    void preorder(BinaryTreeNode<NodeType>* node, std::vector<int>& elements) const;
    bool isSymmetric(const BinaryTreeNode<NodeType>* lnode,
                     const BinaryTreeNode<NodeType>* rnode) const;
    void countByLevel(BinaryTreeNode<NodeType>* node, const std::function<bool(int)>& pred,
                      unsigned int level, std::vector<unsigned int>& nValuesInLevel) const;
    BinaryTreeNode<NodeType>* leastCommonAncestor(BinaryTreeNode<NodeType>* node,
                                                  int a, int b) const;

    BinaryTreeNode<NodeType>* m_root{nullptr}; // made it protected for convenience, generally should not be done!
};

template<typename NodeType>
std::vector<int> BinaryTree<NodeType>::inorderNonRecursive() const
{
    std::vector<int> result;

    std::stack<BinaryTreeNode<NodeType>*> nodeStack;
    BinaryTreeNode<NodeType>* node = m_root;

    while (!nodeStack.empty() || node) {
        if (node) {
            nodeStack.push(node);
            node = node->left;
        }
        else {
            node = nodeStack.top();
            nodeStack.pop();
            result.push_back(node->value);
            node = node->right;
        }
    }

    return result;
}

template<typename NodeType>
std::pair<std::size_t, unsigned int> BinaryTree<NodeType>::maxInLevel(
        const std::function<bool(int)>& pred) const
{
    std::vector<unsigned int> nInLevel;
    this->countByLevel(m_root, pred, 0, nInLevel);

    std::pair<std::size_t, unsigned int> retval{};

    for (std::size_t i = 0; i < nInLevel.size(); ++i) {
        if (nInLevel[i] > retval.second) {
            retval.first = i;
            retval.second = nInLevel[i];
        }
    }

    return retval;
}

template<typename NodeType>
int BinaryTree<NodeType>::height(const BinaryTreeNode<NodeType>* node) const
{
    if (!node)
        return -1;
    return 1 + std::max(this->heightPlus1(node->left), this->heightPlus1(node->right));
}

template<typename NodeType>
void BinaryTree<NodeType>::inorder(BinaryTreeNode<NodeType>* node,
                                   std::vector<int>& elements) const
{
    if (!node)
        return;

    this->inorder(node->left, elements);
    elements.push_back(node->value);
    this->inorder(node->right, elements);
}

template<typename NodeType>
void BinaryTree<NodeType>::preorder(BinaryTreeNode<NodeType>* node,
                                    std::vector<int>& elements) const
{
    if (!node)
        return;

    elements.push_back(node->value);
    this->preorder(node->left, elements);
    this->preorder(node->right, elements);
}

template<typename NodeType>
bool BinaryTree<NodeType>::isSymmetric(const BinaryTreeNode<NodeType>* lnode,
                                       const BinaryTreeNode<NodeType>* rnode) const
{
    if (!lnode && !rnode)
        return true;
    else if (lnode && rnode) {
        return (lnode->value == rnode->value) &&
            this->isSymmetric(lnode->left, rnode->right) &&
            this->isSymmetric(lnode->right, rnode->left);
    }
    else
        return false;
}

template<typename NodeType>
void BinaryTree<NodeType>::countByLevel(BinaryTreeNode<NodeType>* node,
                                        const std::function<bool(int)>& pred,
                                        unsigned int level,
                                        std::vector<unsigned int>& nValuesInLevel) const
{
    if (!node)
        return;

    if (level == nValuesInLevel.size())
        nValuesInLevel.push_back(0);

    if (pred(node->value))
        ++nValuesInLevel[level];

    this->countByLevel(node->left, pred, level + 1, nValuesInLevel);
    this->countByLevel(node->right, pred, level + 1, nValuesInLevel);
}

template<typename NodeType>
BinaryTreeNode<NodeType>* BinaryTree<NodeType>::leastCommonAncestor(BinaryTreeNode<NodeType>* node,
                                                                    int a, int b) const
{
    if (!node)
        return nullptr;
    else if (a == node->value || b == node->value)
        return node;

    BinaryTreeNode<NodeType>* lnode = this->leastCommonAncestor(node->left, a, b);
    BinaryTreeNode<NodeType>* rnode = this->leastCommonAncestor(node->right, a, b);
    if (lnode && rnode)
        return node;
    return lnode ? lnode : rnode;
}

} // namespace psa

#endif // PSA_BINARYTREE_H
