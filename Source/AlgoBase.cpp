/*
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AlgoBase.h"

#include <utility>

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

const int kMaxUlps = 4;
const int kSignBitMaskInt = 1 << (8 * sizeof(int) - 1);
#ifdef WINDOWS
const long long kSignBitMaskLongLong = 1LL << (8 * sizeof(long long) - 1);
#else
const long kSignBitMaskLong = 1L << (8 * sizeof(long) - 1);
#endif

bool isequal(float lhs, float rhs)
{
    static_assert(sizeof(float) == sizeof(int),
                  "isequal() requires size of float and int to be equal.");

    if (std::isnan(lhs) || std::isnan(rhs))
        return false;

    int lhsInt = *reinterpret_cast<int*>(&lhs);
    if (lhsInt < 0)
        lhsInt = kSignBitMaskInt - lhsInt;

    int rhsInt = *reinterpret_cast<int*>(&rhs);
    if (rhsInt < 0)
        rhsInt = kSignBitMaskInt - rhsInt;

    return (std::abs(lhsInt - rhsInt) < kMaxUlps) ? true : false;
}

#ifdef WINDOWS

bool isequal(double lhs, double rhs)
{
    static_assert(sizeof(double) == sizeof(long long),
        "isequal() requires size of double and long to be equal.");

    if (std::isnan(lhs) || std::isnan(rhs))
        return false;

    long long lhsLongLong = *reinterpret_cast<long long*>(&lhs);
    if (lhsLongLong < 0)
        lhsLongLong = kSignBitMaskLongLong - lhsLongLong;

    long long rhsLongLong = *reinterpret_cast<long long*>(&rhs);
    if (rhsLongLong < 0)
        rhsLongLong = kSignBitMaskLongLong - rhsLongLong;

    return (std::abs(lhsLongLong - rhsLongLong) < kMaxUlps) ? true : false;
}

#else

bool isequal(double lhs, double rhs)
{
    static_assert(sizeof(double) == sizeof(long),
        "isequal() requires size of double and long to be equal.");

    if (std::isnan(lhs) || std::isnan(rhs))
        return false;

    long lhsLong = *reinterpret_cast<long*>(&lhs);
    if (lhsLong < 0)
        lhsLong = kSignBitMaskLong - lhsLong;

    long rhsLong = *reinterpret_cast<long*>(&rhs);
    if (rhsLong < 0)
        rhsLong = kSignBitMaskLong - rhsLong;

    return (std::abs(lhsLong - rhsLong) < kMaxUlps) ? true : false;
}

#endif // WINDOWS

/**
* partition routines used in quicksort and order statistics
* p - points to the first element
* r - points to the last element
*/
int *partition(int* p, int* r)
{
    int* i = p;
    for (int* j = p; j < r; ++j) {
        if (*j <= *r) {
            std::swap(*i, *j);
            ++i;
        }
    }
    std::swap(*i, *r);
    return i;
}

/**
* randomized partition
*/
int *randomizedPartition(int* p, int* r)
{
    size_t i = rand() % (r - p + 1);
    std::swap(*(p+i), *r);
    return partition(p, r);
}

/**
* merge routine used for merge sort and counting inversions
*/
long long merge(int* p, int* q, int* r,
                std::vector<int> &leftAuxMem, std::vector<int> &rightAuxMem)
{
    leftAuxMem.clear();
    for (int* k = p; k <= q; ++k)
        leftAuxMem.push_back(*k);
    leftAuxMem.push_back(std::numeric_limits<int>::max());

    rightAuxMem.clear();
    for (int* k = q+1; k <= r; ++k)
        rightAuxMem.push_back(*k);
    rightAuxMem.push_back(std::numeric_limits<int>::max());

    long long inversions = 0;
    int* leftEnd = &leftAuxMem[leftAuxMem.size()-1];

    int* i = &leftAuxMem[0];
    int* j = &rightAuxMem[0];
    for (int* k = p; k <= r; ++k) {
        if (*i <= *j) {
            *k = *i;
            ++i;
        }
        else {
            inversions += leftEnd - i;
            *k = *j;
            ++j;
        }
    }

    return inversions;
}

/**
* merge sort helper
*/
long long mergeSort(int* p, int* r, std::vector<int> &leftAuxMem, std::vector<int> &rightAuxMem)
{
    if (r > p) {
        int* q = p + (r - p) / 2;
        long long leftInversions = mergeSort(p, q, leftAuxMem, rightAuxMem);
        long long rightInversions = mergeSort(q+1, r, leftAuxMem, rightAuxMem);
        long long splitInversions = merge(p, q, r, leftAuxMem, rightAuxMem);
        return leftInversions + rightInversions + splitInversions;
    }
    else
        return 0;
}

#ifdef UNIT_TEST

TEST(AlgoBaseTest, IsEqual)
{
    float a = 1.0f;
    float b = 1.000000000001f;
    float c = 0.999999999999f;
    EXPECT_TRUE(isequal(a, b));
    EXPECT_TRUE(isequal(a, c));

    a = 2341.0f;
    b = 2341.000000000001f;
    c = 2340.999999999999f;
    EXPECT_TRUE(isequal(a, b));
    EXPECT_TRUE(isequal(a, c));

    a = -977.0f;
    b = -977.000000000001f;
    c = -976.999999999999f;
    EXPECT_TRUE(isequal(a, b));
    EXPECT_TRUE(isequal(a, c));

    double d = 1.0f;
    double e = 1.000000000001f;
    double f = 0.999999999999f;
    EXPECT_TRUE(isequal(d, e));
    EXPECT_TRUE(isequal(d, f));

    d = 876.0f;
    e = 876.000000000001f;
    f = 875.999999999999f;
    EXPECT_TRUE(isequal(d, e));
    EXPECT_TRUE(isequal(d, f));
}

#endif // UNIT_TEST

} // namespace psa
