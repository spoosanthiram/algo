/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_QUEUE_H
#define PSA_QUEUE_H

#include <cstddef>

namespace psa{

class Queue
{
public:
    Queue(std::size_t initialCapacity);
    ~Queue();

    void enqueue(int value);
    int dequeue();

    std::size_t size() const { return m_size; }
    std::size_t capacity() const { return m_capacity; }

private:
    void increaseCapacity();

    int* m_queue;
    std::size_t m_istart;
    std::size_t m_size;
    std::size_t m_capacity;
};

} // namespace psa

#endif // PSA_QUEUE_H
