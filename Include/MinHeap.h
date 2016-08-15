/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_MINHEAP_H
#define PSA_MINHEAP_H

#include <functional>
#include <vector>

#include <fmt/format.h>

#include "AlgoBase.h"
#include "AlgoException.h"

namespace psa {

template<typename T>
struct NullIndex
{
    NullIndex() = default;
    NullIndex(const NullIndex& other) {}
    void operator()(const T&, std::size_t) {}
};

/**
 * MinHeap class abstracts the min heap data structure.
 */
template<typename T, typename LessThan = std::less<T>, typename Index = NullIndex<T>>
class MinHeap
{
public:
    MinHeap() = default;
    MinHeap(const std::vector<T>& elements)
        : m_heap{elements}
    {
        this->make();
    }
    MinHeap(std::vector<T>&& elements)
        : m_heap{elements}
    {
        this->make();
    }
    MinHeap(LessThan lt, Index idx)
        : lessThan{lt}
        , index{idx}
    {}
    MinHeap(const std::vector<T>& elements, LessThan lt, Index index)
        : m_heap{elements}
        , lessThan{lt}
        , index{index}
    {
        this->make();
    }
    MinHeap(std::vector<T>&& elements, LessThan lt, Index index)
        : m_heap{elements}
        , lessThan{lt}
        , index{index}
    {
        this->make();
    }

    std::size_t size() const { return m_heap.size(); }
    const T& top() const { return m_heap.front(); }

    bool isEmpty() const { return m_heap.size() == 0; }

    void siftdown(std::size_t i);
    void siftup(std::size_t i);
    void push(const T& element)
    {
        std::size_t i{m_heap.size()};
        index(element, i);
        m_heap.push_back(element);

        this->siftup(i);
    }
    T pop()
    {
        if (m_heap.size() == 0)
            throw AlgoException{"There is no more elements in the heap"};

        T element{m_heap[0]};

        m_heap[0] = m_heap.back();
        index(m_heap[0], 0);
        m_heap.pop_back();

        this->siftdown(0);

        return element;
    }

private:
    void make();

    std::size_t parent(std::size_t i) { return heap_parent(i); }
    std::size_t left(std::size_t i) { return heap_left(i); }
    std::size_t right(std::size_t i) { return heap_right(i); }

    std::vector<T> m_heap{};

    LessThan lessThan{};
    Index index{};
};

template<typename T, typename LessThan, typename Index>
void MinHeap<T, LessThan, Index>::make()
{
    for (std::size_t i = 0; i < m_heap.size(); ++i)
        index(m_heap[i], i);

    for (std::ptrdiff_t i = m_heap.size() / 2 - 1; i >= 0; --i)
        this->siftdown(i);
}

template<typename T, typename LessThan, typename Index>
void MinHeap<T, LessThan, Index>::siftdown(std::size_t i)
{
    std::size_t l = this->left(i);
    std::size_t r = this->right(i);
    std::size_t smallest = i;

    if (l < m_heap.size() && lessThan(m_heap[l], m_heap[i]))
        smallest = l;
    if (r < m_heap.size() && lessThan(m_heap[r], m_heap[smallest]))
        smallest = r;

    if (smallest != i)
    {
        std::swap(m_heap[i], m_heap[smallest]);
        index(m_heap[i], i);
        index(m_heap[smallest], smallest);
        this->siftdown(smallest);
    }
}

template<typename T, typename LessThan, typename Index>
void MinHeap<T, LessThan, Index>::siftup(std::size_t i)
{
    if (i >= m_heap.size())
        throw AlgoException(fmt::format(AlgoException::InvalidIndex,
                                        "MinHeap<>::siftup()", 0, m_heap.size()-1));

    while (i > 0 && lessThan(m_heap[i], m_heap[this->parent(i)]))
    {
        std::swap(m_heap[i], m_heap[this->parent(i)]);
        index(m_heap[i], i);
        index(m_heap[this->parent(i)], this->parent(i));
        i = this->parent(i);
    }
}

} // namespace psa

#endif // PSA_MINHEAP_H
