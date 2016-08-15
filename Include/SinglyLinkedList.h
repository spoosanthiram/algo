/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_SINGLYLINKEDLIST_H
#define PSA_SINGLYLINKEDLIST_H

#include <cstddef>

namespace psa {

class SinglyLinkedList
{
public:
    struct Node {
        Node(int value) : data(value), next(nullptr) {}
        int data;
        Node* next;
    };

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    int front() const { return m_head->data; }
    Node* head() { return m_head; }
    const Node* chead() const { return m_head; }

    std::size_t countNodes() const;

    void insert(int value);

    void merge(SinglyLinkedList& other);
    void reverse();

    bool operator==(const SinglyLinkedList& rhs) const;

private:
    Node* m_head;
};

} // namespace psa

#endif // PSA_SINGLYLINKEDLIST_H
