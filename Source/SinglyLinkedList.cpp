/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "SinglyLinkedList.h"

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

SinglyLinkedList::SinglyLinkedList()
    : m_head(nullptr)
{
}

SinglyLinkedList::~SinglyLinkedList()
{
    while (m_head) {
        Node* tmp = m_head;
        m_head = m_head->next;
        delete tmp;
    }
}

/**
 * @brief SinglyLinkedList::countNodes
 * @return number of nodes aka size
 */
std::size_t SinglyLinkedList::countNodes() const
{
    std::size_t len = 0;

    Node* node = m_head;
    while (node) {
        ++len;
        node = node->next;
    }

    return len;
}

/**
 * @brief insert a node with value in front of the list
 * @param value
 */
void SinglyLinkedList::insert(int value)
{
    Node* node = new Node(value);
    if (m_head) {
        node->next = m_head;
        m_head = node;
    }
    else
        m_head = node;
}

void SinglyLinkedList::merge(SinglyLinkedList& other)
{
    if (!m_head && !other.m_head)
        return;

    Node* newHead = nullptr;

    Node** node = &newHead;
    while (m_head && other.m_head) {
        if (m_head->data < other.m_head->data) {
            *node = m_head;
            m_head = m_head->next;
        }
        else {
            *node = other.m_head;
            other.m_head = other.m_head->next;
        }
        node = &(*node)->next;
    }

    if (m_head)
        *node = m_head;
    else if (other.m_head)
        *node = other.m_head;

    other.m_head = nullptr;

    m_head = newHead;
}

void SinglyLinkedList::reverse()
{
    Node* prevNode = nullptr;
    Node* nextNode = nullptr;
    Node* node = m_head;
    while (node) {
        nextNode = node->next;
        node->next = prevNode;
        prevNode = node;
        node = nextNode;
    }
    m_head = prevNode;
}

bool SinglyLinkedList::operator==(const SinglyLinkedList& rhs) const
{
    bool retval = true;

    Node* lnode = m_head;
    Node* rnode = rhs.m_head;
    while (lnode && rnode) {
        if (lnode->data != rnode->data) {
            retval = false;
            break;
        }
        lnode = lnode->next;
        rnode = rnode->next;
    }

    if (retval && (lnode || rnode))
        retval = false;

    return retval;
}

#ifdef UNIT_TEST

TEST(SinglyLinkedListTest, Merge)
{
    SinglyLinkedList sll;
    sll.insert(9);
    sll.insert(7);
    sll.insert(5);
    sll.insert(3);
    sll.insert(1);

    SinglyLinkedList sll2;
    sll2.insert(8);
    sll2.insert(6);
    sll2.insert(4);
    sll2.insert(2);

    sll.merge(sll2);

    SinglyLinkedList expected;
    expected.insert(9);
    expected.insert(8);
    expected.insert(7);
    expected.insert(6);
    expected.insert(5);
    expected.insert(4);
    expected.insert(3);
    expected.insert(2);
    expected.insert(1);

    EXPECT_EQ(expected, sll);
}

TEST(SinglyLinkedListTest, Reverse)
{
    SinglyLinkedList sll;
    sll.insert(5);
    sll.insert(2);
    sll.insert(4);
    sll.insert(1);

    sll.reverse();

    SinglyLinkedList expected;
    expected.insert(1);
    expected.insert(4);
    expected.insert(2);
    expected.insert(5);

    EXPECT_EQ(expected, sll);
}

#endif // UNIT_TEST

} // namespace psa
