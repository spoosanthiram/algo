/*
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_ALGOEXCEPTION_H
#define PSA_ALGOEXCEPTION_H

#include <exception>
#include <string>

namespace psa {

class AlgoException : public std::exception
{
public:
    static const char* InvalidIndex;
    static const char* FileOpenRead;

    static const char* StackUnderflow;

    // Graph
    static const char* GraphBadFormat;

    // Matrix
    static const char* MatrixZeroDimension;
    static const char* MatrixDimMismatchForAdd;
    static const char* MatrixDimMismatchForMult;

    // HashTable
    static const char* HashTableKeyAlreadyPresent;
    static const char* HashTableKeyNotFound;

public:
    AlgoException(const std::string& message)
        : std::exception{}
        , m_message{message}
    {}

    const char* what() const noexcept override { return m_message.c_str(); }

protected:
    std::string m_message;
};

} // namespace psa

#endif // PSA_ALGOEXCEPTION_H
