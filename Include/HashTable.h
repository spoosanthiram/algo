/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_HASHTABLE_H
#define PSA_HASHTABLE_H

#include <forward_list>

#include <fmt/format.h>

#include "AlgoException.h"

namespace psa {

class HashTable {
public:
    HashTable(unsigned int nbuckets);
    ~HashTable() { delete [] m_buckets; }

    bool isExist(unsigned int key) const
    {
        bucket_t& bucket = this->bucket(key);
        iterator_t it = this->iterator(bucket, key);
        return it != bucket.end();
    }

    int itemAt(unsigned int key) const
    {
        bucket_t& bucket = this->bucket(key);
        iterator_t it = this->iterator(bucket, key);
        if (it == bucket.end())
            throw AlgoException{fmt::format(AlgoException::HashTableKeyNotFound, key)};

        return (*it).second;
    }

    void insert(unsigned int key, int value)
    {
        bucket_t& bucket = this->bucket(key);
        iterator_t it = this->iterator(bucket, key);
        if (it != bucket.end())
            throw AlgoException{fmt::format(AlgoException::HashTableKeyAlreadyPresent, key)};

        bucket.emplace_front(key, value);
        ++m_size;
    }

    void remove(unsigned int key)
    {
        bucket_t& bucket = this->bucket(key);
        iterator_t it = this->iterator(bucket, key);
        if (it == bucket.end())
            throw AlgoException{fmt::format(AlgoException::HashTableKeyNotFound, key)};

        bucket.remove(*it);
        --m_size;
    }

private:
    using item_t = std::pair<unsigned int, int>;
    using bucket_t = std::forward_list<item_t>;
    using iterator_t = bucket_t::iterator;

    unsigned int primeBucketSize(unsigned int nbuckets);
    bucket_t& bucket(unsigned int key) const
    {
        unsigned int index = key % m_nbuckets;
        return m_buckets[index];
    }
    iterator_t iterator(bucket_t& bucket, unsigned int key) const;

    bucket_t* m_buckets;
    std::size_t m_nbuckets;
    std::size_t m_size;
};

} // namespace psa

#endif // PSA_HASHTABLE_H
