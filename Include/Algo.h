/**
 * Header file for common algorithms
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_ALGO_H
#define PSA_ALGO_H

#include <string>
#include <vector>

namespace psa {

constexpr bool isLeapYear(unsigned int year);

long long fibonacci(unsigned int n);

std::size_t linearSearch(const std::vector<int>& a, int x);
std::size_t binarySearch(const std::vector<int>& a, int x);

long maximumSubarray(const std::vector<int>& a, std::size_t& startIndex, std::size_t& endIndex);

int select(int* p, int* r, std::size_t i);
int select(std::vector<int>& a, std::size_t i);

std::vector<int> merge(const std::vector<int>& seg1, const std::vector<int>& seg2);

long long inversions(std::vector<int>& a);

std::vector<std::string> permutations(const std::string& str);

bool isnumber(char c);
bool isvalidalpha(char c);
char tolower(char c);
unsigned int stoi(const std::string& str, int base);
std::string toStringBaseP2(unsigned int x, int base);
std::string toString(unsigned int x, int base);

bool isPrime(unsigned int n);

bool isAdditive(const std::string& numstr);
bool isAdditive(const std::string& numstr, std::size_t indexToStart,
                std::size_t len1, std::size_t len2);

struct Job
{
    Job(int w, int l) : weight{ w }, length{ l } {}
    int weight;
    int length;
};
inline bool jobsByDifference(const Job& x, const Job& y)
{
    int xval = x.weight - x.length;
    int yval = y.weight - y.length;
    return xval == yval ? x.weight > y.weight : xval > yval;
}
inline bool jobsByRatio(const Job& x, const Job& y)
{
    return static_cast<double>(x.weight) / x.length >
        static_cast<double>(y.weight) / y.length;
}
#ifdef WINDOWS
long long computeWeightedCompletionTime(const std::vector<Job>& jobs);
#else
long computeWeightedCompletionTime(const std::vector<Job>& jobs);
#endif // WINDOWS

#ifdef UNIT_TEST
std::vector<int> readIntegers(const std::string& filePath);
std::vector<Job> readJobs(const std::string& filePath);
#endif

} // namespace psa

#endif // PSA_ALGO_H
