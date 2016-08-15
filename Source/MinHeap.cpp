/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "MinHeap.h"

#include <algorithm>

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

#ifdef UNIT_TEST

TEST(MinHeap, Simple)
{
    MinHeap<int> mh{};

    using IntCompare = bool(*)(int, int);
    using IntIndex = void(*)(int, std::size_t);

    auto intCompare = [](int lhs, int rhs) -> bool { return lhs < rhs; };
    auto intIndex = [](int element, std::size_t i) {};

    MinHeap<int, IntCompare, IntIndex> mh2{intCompare, intIndex};
}

#endif // UNIT_TEST

} // namespace psa
