/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BinarySearchTree.h"

#ifdef UNIT_TEST
#include <algorithm>

#include "gtest/gtest.h"
#endif

namespace psa {

#ifdef UNIT_TEST

TEST(BinarySearchTreeTest, Inorder)
{
    std::vector<int> values{6, 5, 7, 2, 5, 8};
    BinarySearchTree<int> tree{values};
    std::vector<int> actual = tree.inorder();
    std::sort(values.begin(), values.end());
    EXPECT_EQ(values, actual);
}

TEST(BinarySearchTreeTest, InorderNonRecursive)
{
    std::vector<int> values{6, 5, 7, 2, 5, 8};
    BinarySearchTree<int> tree{values};
    std::vector<int> actual = tree.inorderNonRecursive();
    std::sort(values.begin(), values.end());
    EXPECT_EQ(values, actual);
}

TEST(BinarySearchTreeTest, Preorder)
{
    std::vector<int> values{6, 5, 7, 2, 5, 8};
    BinarySearchTree<int> tree(values);
    std::vector<int> actual = tree.preorder();
    std::vector<int> expected{6, 5, 2, 5, 7, 8};
    EXPECT_EQ(expected, actual);
}

TEST(BinarySearchTreeTest, MostNegativeInLevel)
{
    std::vector<int> values{-5, -7, -2, -3, 0};
    BinarySearchTree<int> tree(values);
    auto actual = tree.maxInLevel([](int value) { return value < 0; });
    std::pair<std::size_t, unsigned int> expected{1, 2};
    EXPECT_EQ(expected, actual);
}

#endif // UNIT_TEST

} // namespace psa
