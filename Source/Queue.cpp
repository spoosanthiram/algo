/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Queue.h"

#include "AlgoException.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace psa {

Queue::Queue(std::size_t initialCapacity)
    : m_queue{nullptr}
    , m_istart{0}
    , m_size{0}
    , m_capacity{initialCapacity}
{
    if (0 == m_capacity)
        m_capacity = 1;

    m_queue = new int[m_capacity];
}

Queue::~Queue()
{
    delete[] m_queue;
}

void Queue::enqueue(int value)
{
    if (m_size < m_capacity) {
        m_queue[(m_istart + m_size) % m_capacity] = value;
        ++m_size;
    }
    else {
        this->increaseCapacity();
        this->enqueue(value);
    }
}

int Queue::dequeue()
{
    if (m_size <= 0)
        throw AlgoException("There is no elements in the queue.");

    int retval = m_queue[m_istart];
    m_istart = ++m_istart % m_capacity;
    --m_size;
    return retval;
}

void Queue::increaseCapacity()
{
    std::size_t newCapacity = 2 * m_capacity;
    int* tmpQueue = new int[newCapacity];
    for (std::size_t i = 0; i < m_size; ++i)
        tmpQueue[i] = m_queue[(m_istart + i) % m_capacity];

    delete[] m_queue;

    m_queue = tmpQueue;
    m_istart = 0;
    m_capacity = newCapacity;
}

#ifdef UNIT_TEST

TEST(QueueTest, Enqueue)
{
    Queue q{2};
    q.enqueue(234);
    q.enqueue(453);
    q.enqueue(434);
}

#endif // UNIT_TEST

} // namespace psa
