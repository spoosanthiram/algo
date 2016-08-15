/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Sorting.h"

#include "AlgoBase.h"

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

/**
 * insertion sort
 */
void insertionSort(std::vector<int>& a)
{
    if (a.size() <= 1)
        return;

    int* pa = a.data();

    int* p = pa + 1;
    int* r = pa + a.size();
    int* q;
    for (; p < r; ++p) {
        int val = *p;
        for (q = p - 1; q >= pa && *q > val; --q)
            *(q + 1) = *q;
        *(++q) = val;
    }
}

/**
 * merge sort
 */
void mergeSort(std::vector<int>& a)
{
    if (a.size() <= 1)
        return;

    std::vector<int> leftAuxMem, rightAuxMem;
    leftAuxMem.reserve(a.size()/2 + 2);
    rightAuxMem.reserve(a.size()/2 + 2);

    mergeSort(&a[0], &a[a.size() - 1], leftAuxMem, rightAuxMem);
}

/**
 * quick sort
 */
std::ptrdiff_t quickSort(int* p, int* r)
{
    if (p < r) {
        int* q = partition(p, r);
        std::ptrdiff_t leftComparisons = quickSort(p, q - 1);
        std::ptrdiff_t rightComparisons = quickSort(q + 1, r);
        return leftComparisons + rightComparisons + (r - p);
    }
    else
        return 0;
}

std::ptrdiff_t quickSort(std::vector<int>& a)
{
    if (a.size() <= 1)
        return 0;

    int* pa = a.data();
    return quickSort(pa, pa + (a.size() - 1));
}

/**
 * randomized quicksort
 */
std::ptrdiff_t randomizedQuickSort(int* p, int* r)
{
    if (p < r) {
        int* q = randomizedPartition(p, r);
        std::ptrdiff_t leftComparisons = randomizedQuickSort(p, q - 1);
        std::ptrdiff_t rightComparisons = randomizedQuickSort(q + 1, r);
        return leftComparisons + rightComparisons + (r - p);
    }
    else
        return 0;
}

std::ptrdiff_t randomizedQuickSort(std::vector<int>& a)
{
    if (a.size() <= 1)
        return 0;

    int* data = a.data();
    return randomizedQuickSort(data, data + (a.size() - 1));
}

#ifdef UNIT_TEST

TEST(SortingTest, InsersionSort)
{
    std::vector<int> a{3, 5, 7, 2, 9, 6, 8, 5, 1};
    psa::insertionSort(a);
    std::vector<int> expected{1, 2, 3, 5, 5, 6, 7, 8, 9};
    EXPECT_EQ(expected, a);
}

TEST(SortingTest, MergeSort)
{
    std::vector<int> a{5, 4, 10, 6, 13, 9, 1, 11, 9, 14, 12, 3, 0, 2, 7};
    psa::mergeSort(a);
    std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 9, 9, 10, 11, 12, 13, 14};
    EXPECT_EQ(expected, a);
}

TEST(SortingTest, QuickSort)
{
    std::vector<int> a{5, 1, 6, 3, 7, 2, 8, 4};
    psa::quickSort(a);
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(expected, a);
}

#endif // UNIT_TEST

} // namespace psa
