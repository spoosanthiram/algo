/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "HashTable.h"

#include <cmath>

#include "Algo.h"

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

HashTable::HashTable(unsigned int nbuckets)
    : m_buckets{nullptr}
    , m_nbuckets{0}
    , m_size{0}
{
    m_nbuckets = this->primeBucketSize(nbuckets);
    m_buckets = new std::forward_list<std::pair<unsigned int, int>>[m_nbuckets];
}

unsigned int HashTable::primeBucketSize(unsigned int nbuckets)
{
    double power = std::log2(nbuckets);
    unsigned int low = static_cast<unsigned int>(std::pow(2.0, std::floor(power)));
    unsigned int high = static_cast<unsigned int>(std::pow(2.0, std::ceil(power)));

    unsigned int acceptableSize = low + (high - low) / 2;

    if (nbuckets > acceptableSize) {
        for (unsigned int n = acceptableSize; n < high; ++n) {
            if (psa::isPrime(n)) {
                acceptableSize = n;
                break;
            }
        }
    } else {
        for (unsigned int n = acceptableSize; n > low; --n) {
            if (psa::isPrime(n)) {
                acceptableSize = n;
                break;
            }
        }
    }

    return acceptableSize;
}

HashTable::iterator_t HashTable::iterator(bucket_t& bucket, unsigned int key) const
{
    auto it = bucket.begin();
    for (; it != bucket.end(); ++it) {
        if ((*it).first == key)
            break;
    }

    return it;
}

#ifdef UNIT_TEST

TEST(HashTableTest, Simple)
{
    HashTable table{100};
    table.insert(12341, 1);
    table.insert(65748, 2);

    EXPECT_EQ(1, table.itemAt(12341));
    EXPECT_EQ(2, table.itemAt(65748));

    EXPECT_THROW(table.itemAt(3452), AlgoException);
}

TEST(HashTableTest, AlgoClass2Sum)
{
    const int t1 = 2500;
    const int t2 = 4000;

    std::vector<int> integers = readIntegers("AlgoClass2Sum.txt");

    unsigned int nprobable = 0;
    for (auto n : integers) {
        if (n <= t2)
            ++nprobable;
    }

    HashTable table{nprobable};
    for (auto n : integers) {
        if (n <= t2) {
            try {
                table.insert(n, 1);
            } catch(...) {}
        }
    }

    unsigned int result = 0;

    for (auto n : integers) {
        if (n > t2)
            continue;

        if (table.isExist(n)) {
            for (int t = t1; t <= t2; ++t) {
                int n2 = std::abs(n - t);
                if (n != n2 && table.isExist(n2)) {
                    ++result;
                    break;
                }
            }
        }
    }

    std::cout << "2-sum result: " << result << std::endl;
}

#endif // UNIT_TEST

} // namespace psa
