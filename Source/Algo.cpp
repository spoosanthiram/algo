/**
 * Source file for common algorithms
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Algo.h"

#include <algorithm>
#include <cmath>

#include <fmt/format.h>

#include "AlgoBase.h"
#include "AlgoException.h"
#include "Matrix.h"

#ifdef UNIT_TEST
#include <fstream>
#include <regex>

#include <gtest/gtest.h>
#endif

namespace {

const char kDigitLookup[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

const unsigned int kComputedPrimesSize = 500;
unsigned int* s_computedPrimes = nullptr;

unsigned int* computedPrimes()
{
    if (s_computedPrimes)
        return s_computedPrimes;

    s_computedPrimes = new unsigned int[kComputedPrimesSize];

    s_computedPrimes[0] = 2;
    s_computedPrimes[1] = 3;

    unsigned int nprimes = 2;

    // cycle numbers from 4 util we find kComputedPrimesSize primes
    for (unsigned int n = 4; nprimes < kComputedPrimesSize; ++n) {
        unsigned int sqrtN = static_cast<unsigned int>(std::sqrt(n));
        bool nIsPrime = true;

        for (unsigned int i = 0; i < nprimes && s_computedPrimes[i] <= sqrtN; ++i) {
            if (n % s_computedPrimes[i] == 0) {
                nIsPrime = false;
                break;
            }
        }

        if (nIsPrime) {
            s_computedPrimes[nprimes] = n;
            ++nprimes;
        }
    }

    return s_computedPrimes;
}

} // namespace

namespace psa {

/**
* Check the <code>year</code> is leap year or not
*/
constexpr bool isLeapYear(unsigned int year)
{
    return ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0))) ? true : false;
}

/**
* Computes fibonacci number
*/
long long fibonacci(unsigned int n)
{
    long long f0{0};
    long long f1{1};

    if (n == 0) return f0;
    if (n == 1) return f1;

    long long fibMatrix[] = { 0, 1, 1, 1 };
    Matrix<long long> M = Matrix<long long>::identity(2);
    Matrix<long long> ithFib(2, 2, fibMatrix);

    while (n > 0) {
        if ((n & 0x1) > 0)
            M *= ithFib;

        ithFib *= ithFib;
        n >>= 1;
    }

    long long fn = M.valueAt(0, 0) * f0 + M.valueAt(0, 1) * f1;
    // long long fnPlus1 = M[2] * f0 + M[3] * f1;

    return fn;
}

std::size_t linearSearch(const std::vector<int>& a, int x)
{
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] == x)
            return i;
    }

    return -1;
}

/**
* binary search
* a - sorted (non-decreasing) input array
* key - key to search
* Returns index of the element if found otherwise returns -1
*/
std::size_t binarySearch(const std::vector<int>& a, int x)
{
    if (a.size() <= 0)
        return -1;

    const int* adata = a.data();

    const int* l = adata;
    const int* u = &adata[a.size() - 1];
    while (l <= u) {
        const int* m = l + (u - l + 1) / 2;
        if (x == *m)
            return m - adata;
        else if (x < *m)
            u = m - 1;
        else
            l = m + 1;
    }

    return -1;
}

/**
* Finds the maximum sub array.
* a - input array
* Returns, maximum sub array value. 0 implies the result set is empty.
* Otherwise, startIndex and endIndex will point to the starting and
* ending indices of maximum sub array.
*/
long maximumSubarray(const std::vector<int>& a, std::size_t& startIndex, std::size_t& endIndex)
{
    if (a.size() <= 0)
        return 0;

    const int* adata = a.data();

    startIndex = endIndex = 0;

    long maxEndingHere{0};
    long maxSoFar{0};
    const int* startIndexCandidate = adata;

    const int* end = adata + a.size();
    for (const int* i = adata; i < end; ++i) {
        maxEndingHere += *i;

        if (maxEndingHere < 0) {
            maxEndingHere = 0;
            startIndexCandidate = i + 1;
        }

        if (maxEndingHere > maxSoFar) {
            maxSoFar = maxEndingHere;
            startIndex = startIndexCandidate - adata;
            endIndex = i - adata;
        }
    }

    return maxSoFar;
}

/**
* helper function for select (order statistics)
*/
int select(int* p, int* r, std::size_t i)
{
    if (p == r)
        return *p;

    int* q = randomizedPartition(p, r);
    size_t k = q - p;
    if (i == k)
        return *q;
    else if (i < k)
        return select(p, q-1, i);
    else
        return select(q+1, r, i-k-1);
}

/**
* Selects the ith value from the unsorted sequence.
* a - input array
* i - ith value to find
* Returns the value at ith index for the sequence if it were sorted.
*/
int select(std::vector<int>& a, std::size_t i)
{
    if (a.size() == 0 || i >= a.size())
        throw AlgoException(fmt::format(AlgoException::InvalidIndex, "select()", 0, a.size()-1));

    int* adata = a.data();
    return select(&adata[0], &adata[a.size() - 1], i);
}

/**
 * Generic merge that merges two sorted subarrays
 * Note: It is assumed that both subarrays have std::numeric_limits<int>::max() as the last element.
 */
std::vector<int> merge(const std::vector<int>& seg1, const std::vector<int>& seg2)
{
    std::vector<int> result;

    std::size_t size = seg1.size() + seg2.size() - 2;
    if (size > 0)
        result.reserve(size);

    auto it1 = seg1.begin();
    auto it2 = seg2.begin();
    for (std::size_t i = 0; i < size; ++i) {
        if (*it1 < *it2)
            result.push_back(*it1++);
        else
            result.push_back(*it2++);
    }

    return result;
}

/**
 * Counts the inversions
 */
long long inversions(std::vector<int>& a)
{
    if (a.size() <= 1)
        return 0;

    std::vector<int> leftAuxMem;
    leftAuxMem.reserve(a.size()/2 + 2);

    std::vector<int> rightAuxMem;
    rightAuxMem.reserve(a.size()/2 + 2);

    return mergeSort(&a[0], &a[a.size() - 1], leftAuxMem, rightAuxMem);
}

/**
* Returns the permutations
*/
std::vector<std::string> permutations(const std::string& str)
{
    if (str.size() == 0)
        return std::vector<std::string>();

    std::vector<std::string> words;

    if (str.size() == 1)
        words.push_back(str);
    else {
        std::vector<std::string> wordList = permutations(str.substr(1));

        std::string firstChar = str.substr(0, 1);

        for (auto& word : wordList) {
            std::size_t n = word.size();
            for (std::size_t i = 0; i <= n; ++i)
                words.push_back(word.substr(0, i) + firstChar + word.substr(i));
        }
    }

    return words;
}

inline bool isnumber(char c)
{
    return c >= '0' && c <= '9';
}

inline bool isvalidalpha(char c)
{
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

inline char tolower(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

unsigned int stoi(const std::string& str, int base)
{
    unsigned int result = 0;

    int d = 0;
    for (char c : str) {
        if (isnumber(c))
            d = c - '0';
        else if (isvalidalpha(c))
            d = tolower(c) - 'a' + 10;
        else
            break; // invalid char

        if (d >= base) break; // invalid for the base

        result = result * base + d;
    }

    return result;
}

std::string toStringBaseP2(unsigned int x, int base)
{
    std::string result{};

    if (base < 2 || base > 16 || base & (base - 1)) // check for base and power of 2
        return result;

    int mask = base - 1;
    int nshift = 1;
    while (base > 2) {
        ++nshift;
        base /= 2;
    }

    while (x) {
        result.push_back(kDigitLookup[x & mask]);
        x >>= nshift;
    }

    std::reverse(result.begin(), result.end());

    return result;
}

std::string toString(unsigned int x, int base)
{
    std::string result{};

    if (base < 2 || base > 16)
        return result;

    if (!(base & (base - 1)))
        result = toStringBaseP2(x, base);
    else {
        while (x) {
            result.push_back(kDigitLookup[x % base]);
            x /= base;
        }

        std::reverse(result.begin(), result.end());
    }

    return result;
}

bool isPrime(unsigned int n)
{
    unsigned int* primes = computedPrimes();

    if (n < primes[0])
        return false;

    unsigned int lastComputedPrime = primes[kComputedPrimesSize - 1];

    if (n > lastComputedPrime * lastComputedPrime)
        throw AlgoException{
            fmt::format("isPrime() is not designed to check a number bigger than {}.",
            lastComputedPrime * lastComputedPrime)};

    if (n <= lastComputedPrime)
        return std::binary_search(&primes[0],
                                  &primes[kComputedPrimesSize],
                                  n);

    for (unsigned int i = 0; i < kComputedPrimesSize; ++i) {
        if (n % primes[i] == 0)
            return false;
    }

    return true;
}

bool isAdditive(const std::string& numstr)
{
    if (numstr.size() < 3)
        throw AlgoException{fmt::format("isAdditive() need at least 3 digits, given '{}'.",
                                        numstr)};

    std::size_t sizeBy2 = (numstr.size() + 1) / 2;

    for (std::size_t k = 2; k <= sizeBy2; ++k) {
        for (std::size_t i = 1, j = k - 1; i < k; ++i, --j) {
            if (isAdditive(numstr, 0, i, j))
                return true;
        }
    }

    return false;
}

bool isAdditive(const std::string& numstr, std::size_t indexToStart,
                std::size_t len1, std::size_t len2)
{
    std::string firstNumStr = numstr.substr(indexToStart, len1);
    std::string secondNumStr = numstr.substr(indexToStart + len1, len2);
    int thirdNum = std::stoi(firstNumStr) + std::stoi(secondNumStr);
    std::string thirdNumStr = std::to_string(thirdNum);
    if (numstr.size() < (indexToStart + len1 + len2 + thirdNumStr.size()) ||
        numstr.compare(indexToStart + len1 + len2, thirdNumStr.size(), thirdNumStr) != 0)
    {
        return false;
    }
    else {
        if (numstr.size() == indexToStart + len1 + len2 + thirdNumStr.size())
            return true;
        else
            return isAdditive(numstr, indexToStart + len1, len2, thirdNumStr.size());
    }
}

#ifdef WINDOWS
long long computeWeightedCompletionTime(const std::vector<Job>& jobs)
#else
long computeWeightedCompletionTime(const std::vector<Job>& jobs)
#endif // WINDOWS
{
#ifdef WINDOWS
    long long weightedCompletionTime = 0;
    long long completionTime = 0;
#else
    long weightedCompletionTime = 0;
    long completionTime = 0;
#endif // WINDOWS
    for (const Job& job : jobs) {
        completionTime += job.length;
        weightedCompletionTime += job.weight * completionTime;
    }

    return weightedCompletionTime;
}

#ifdef UNIT_TEST

std::vector<int> readIntegers(const std::string& filePath)
{
    std::ifstream inputStream{filePath};
    if (!inputStream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filePath)};

    std::vector<int> integers;

    std::string line;
    while (!inputStream.eof()) {
        std::getline(inputStream, line);
        if (line.size() > 0)
            integers.push_back(std::stoi(line));
    }

    return integers;
}

std::vector<Job> readJobs(const std::string& filePath)
{
    std::ifstream inputStream{filePath};
    if (!inputStream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filePath)};

    std::vector<Job> jobs;

    std::string line;
    while (!inputStream.eof() && line.size() <= 0)
        std::getline(inputStream, line);

    int njobs = (line.size() > 0) ? std::stoi(line) : 0;

    std::regex wsRe{R"(\s+)"};
    int jobValues[2];

    while (!inputStream.eof() && njobs > 0) {
        std::getline(inputStream, line);
        if (line.size() > 0) {
            jobValues[0] = 0;
            jobValues[1] = 0;
            auto it = std::sregex_token_iterator{line.begin(), line.end(), wsRe, -1};
            for (int i = 0; it != std::sregex_token_iterator{}; ++i, ++it) {
                jobValues[i] = std::stoi(it->str());
            }
            jobs.emplace_back(jobValues[0], jobValues[1]);
            --njobs;
        }
    }

    return jobs;
}

TEST(AlgoTest, LeapYear)
{
    EXPECT_FALSE(isLeapYear(1799));
    EXPECT_FALSE(isLeapYear(1800));
    EXPECT_TRUE(isLeapYear(2000));
    EXPECT_TRUE(isLeapYear(2012));
    EXPECT_FALSE(isLeapYear(2013));
    EXPECT_FALSE(isLeapYear(2014));
    EXPECT_FALSE(isLeapYear(2100));
}

TEST(AlgoTest, Fibonacci)
{
    ASSERT_EQ(0, psa::fibonacci(0));
    ASSERT_EQ(1, psa::fibonacci(1));
    EXPECT_EQ(5, psa::fibonacci(5));
    EXPECT_EQ(144, psa::fibonacci(12));
    EXPECT_EQ(6765, psa::fibonacci(20));
    EXPECT_EQ(196418, psa::fibonacci(27));
    EXPECT_EQ(1548008755920L, psa::fibonacci(60));
}

TEST(AlgoTest, LinearSearch)
{
    std::vector<int> a{4, 9, 1, 8, 7, 3, 2, 5, 6};
    EXPECT_EQ(4, linearSearch(a, 7));
    EXPECT_EQ(-1, linearSearch(a, 89));
}

TEST(AlgoTest, BinarySearch)
{
    int aValues[] = { 2, 9, 17, 34, 56, 234, 564, 789, 900, 1092 };
    std::vector<int> a(&aValues[0], &aValues[sizeof(aValues)/sizeof(int)]);
    std::ptrdiff_t actual = psa::binarySearch(a, 564);
    std::ptrdiff_t expected = 6;
    EXPECT_EQ(expected, actual);
}

TEST(AlgoTest, MaximumSubarray)
{
    int aValues[] = {
        13,  -3, -25,  20,  -3,
        -16, -23,  18,  20,  -7,
        12,  -5, -22,  15,  -4,
        7
    };
    std::vector<int> a(&aValues[0], &aValues[sizeof(aValues)/sizeof(int)]);

    size_t istart, iend;
    long sum = psa::maximumSubarray(a, istart, iend);
    EXPECT_EQ(7u, istart);
    EXPECT_EQ(10u, iend);
    EXPECT_EQ(43u, sum);
}

TEST(AlgoTest, Selection)
{
    std::vector<int> a{3, 2, 9, 0, 7, 5, 4, 8, 6, 1};
    ptrdiff_t val = psa::select(a, 7);
    EXPECT_EQ(7, val);
}

TEST(AlgoBaseTest, Merge)
{
    std::vector<int> segment1{1, 3, 5, 7, 8};
    std::vector<int> segment2{2, 4, 6, 9};

    segment1.push_back(std::numeric_limits<int>::max());
    segment2.push_back(std::numeric_limits<int>::max());

    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> actual = merge(segment1, segment2);
    EXPECT_EQ(expected, actual);
}

TEST(AlgoTest, Inversions)
{
    std::vector<int> integers{1, 3, 5, 2, 4, 6};
    long long inv = psa::inversions(integers);
    EXPECT_EQ(3, inv);
}

TEST(AlgoTest, AlgoClassInversions)
{
    std::vector<int> integers = readIntegers("AlgoClassInversionsInput.txt");
    long long ninversions = psa::inversions(integers);
    long long expected = 2407905288;
    EXPECT_EQ(expected, ninversions);
}

TEST(AlgoTest, Permutations)
{
    std::vector<std::string> expected;
    expected.push_back("abc");
    expected.push_back("bac");
    expected.push_back("bca");
    expected.push_back("acb");
    expected.push_back("cab");
    expected.push_back("cba");
    std::sort(expected.begin(), expected.end());

    std::vector<std::string> actual = psa::permutations("abc");
    std::sort(actual.begin(), actual.end());

    EXPECT_TRUE(expected == actual);
}

TEST(AlgoTest, StrToInt)
{
    EXPECT_EQ(142u, stoi("142", 10));
    EXPECT_EQ(0xacu, stoi("10101100", 2));
    EXPECT_EQ(0x3a7b9eu, stoi("3a7b9e", 16));
}

TEST(AlgoTest, IntToStr)
{
    EXPECT_EQ("322089", toString(322089, 10));
    EXPECT_EQ("1101100001101010", toString(0xd86a, 2));
    EXPECT_EQ("31", toString(13, 4));
}

TEST(AlgoTest, Additive)
{
    EXPECT_TRUE(isAdditive("1123581321345589144"));
    EXPECT_FALSE(isAdditive("112358132"));
    EXPECT_TRUE(isAdditive("199100199"));
}

TEST(AlgoTest, ScheduleJobs)
{
    std::vector<Job> jobs = readJobs("AlgoClassScheduleJobs.txt");

    std::sort(jobs.begin(), jobs.end(), jobsByDifference);
    long long expected = 69119377652;
    EXPECT_EQ(expected, computeWeightedCompletionTime(jobs));

    std::sort(jobs.begin(), jobs.end(), jobsByRatio);
    expected = 67311454237;
    EXPECT_EQ(expected, computeWeightedCompletionTime(jobs));
}

#endif // UNIT_TEST

} // namespace psa
