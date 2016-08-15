/**
 * Header file for Max Tracking Stack
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_MAXTRACKINGSTACK_H
#define PSA_MAXTRACKINGSTACK_H

#include <vector>

namespace psa {

class MaxTrackingStack {
public:
    void push(int value);
    int pop();
    int max() const;

private:
    std::vector<std::pair<int, int>> m_container{};
};

} // namespace psa

#endif // PSA_MAXTRACKINGSTACK_H
