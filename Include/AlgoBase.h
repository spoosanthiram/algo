/*
 * Header file for the basic stuff that are used all over
 * some implementations inlined
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_ALGOBASE_H
#define PSA_ALGOBASE_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <vector>

namespace psa {

const double kPi = 3.14159265358979323846264338327950288;
const double kRadianMultiplier = kPi / 180.0;
const double kDegreeMultiplier = 180.0 / kPi;

/**
 * check the number is epsilon close to zero
 */
inline bool iszero(float value)
{
    return std::abs(value) < std::numeric_limits<float>::epsilon();
}
inline bool iszero(double value)
{
    return std::abs(value) < std::numeric_limits<double>::epsilon();
}

bool isequal(float lhs, float rhs);
bool isequal(double lhs, double rhs);

inline bool isPositiveInfinite(float value)
{
    return std::numeric_limits<float>::infinity() == value;
}
inline bool isPositiveInfinite(double value)
{
    return std::numeric_limits<double>::infinity() == value;
}

inline bool isNegativeInfinite(float value)
{
    return -std::numeric_limits<float>::infinity() == value;
}
inline bool isNegativeInfinite(double value)
{
    return -std::numeric_limits<double>::infinity() == value;
}

inline bool isInfinite(float value)
{
    return isPositiveInfinite(value) || isNegativeInfinite(value);
}
inline bool isInfinite(double value)
{
    return isPositiveInfinite(value) || isNegativeInfinite(value);
}

/**
 * add two single precision values taking into account the values could be infinite
 */
template<typename T>
inline T add(T lhs, T rhs)
{
    if (isInfinite(lhs) && isInfinite(rhs))
        return ((isPositiveInfinite(lhs) && isPositiveInfinite(rhs)) ||
                (isNegativeInfinite(lhs) && isNegativeInfinite(rhs))) ? lhs + rhs : lhs;
    else
        return lhs + rhs;
}

template<typename T>
inline T multiply(T lhs, T rhs)
{
    return (iszero(lhs) || iszero(rhs)) ? 0.0f : lhs * rhs;
}

inline double normalizeAngle(double angle)
{
    if (angle >= -180.0 && angle <= 180.0)
        return angle;

    angle = std::fmod(angle, 360.0);

    if (angle >= 180.0)
        angle -= 360.0;
    if (angle < -180.0)
        angle += 360.0;

    return angle;
}

inline double cos(double angle)
{
    angle = normalizeAngle(angle);

    if (iszero(angle))
        return 1.0;
    else if (isequal(std::abs(angle), 90.0))
        return 0.0;
    else if (isequal(std::abs(angle), 180.0))
        return -1.0;
    else
        return std::cos(angle * psa::kRadianMultiplier);
}

inline double sin(double angle)
{
    angle = normalizeAngle(angle);

    if (iszero(angle) || iszero(std::abs(angle) - 180.0))
        return 0.0;
    else if (iszero(angle - 90.0))
        return 1.0;
    else if (iszero(angle + 90.0))
        return -1.0;
    else
        return std::sin(angle * psa::kRadianMultiplier);
}

int *partition(int* p, int* r);
int *randomizedPartition(int* p, int* r);

long long merge(int* p, int* q, int* r,
                std::vector<int> &leftAuxMem, std::vector<int> &rightAuxMem);
long long mergeSort(int* p, int* r, std::vector<int> &leftAuxMem, std::vector<int> &rightAuxMem);

/**
 * functions for heap operation.
 */
inline std::size_t heap_parent(std::size_t i) { return (i - 1) / 2; }
inline std::size_t heap_left(std::size_t i) { return 2 * i + 1; }
inline std::size_t heap_right(std::size_t i) { return 2 * i + 2; }

} // namespace psa

#endif // PSA_ALGOBASE_H
