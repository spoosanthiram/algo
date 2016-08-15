/**
 * Source file for Max Tracking Stack
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "MaxTrackingStack.h"

#include <algorithm>

#include "AlgoException.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace psa {

void MaxTrackingStack::push(int value)
{
    int maxVal = std::max(value, m_container.empty() ? value : m_container.back().second);
    m_container.emplace_back(value, maxVal);
}

int MaxTrackingStack::pop()
{
    if (m_container.empty())
        throw AlgoException{AlgoException::StackUnderflow};

    int retval = m_container.back().first;
    m_container.pop_back();
    return retval;
}

int MaxTrackingStack::max() const
{
    if (m_container.empty())
        throw AlgoException{AlgoException::StackUnderflow};

    return m_container.back().second;
}

#ifdef UNIT_TEST

TEST(MaxTrackingStackTest, Simple)
{
    MaxTrackingStack mts;
    mts.push(12);
    mts.push(3);
    EXPECT_EQ(12, mts.max());
    mts.push(45);
    EXPECT_EQ(45, mts.max());
    mts.pop();
    mts.pop();
    EXPECT_EQ(12, mts.max());
    mts.pop();

    EXPECT_THROW(mts.max(), AlgoException);
}

#endif // UNIT_TEST

} // namespace psa
