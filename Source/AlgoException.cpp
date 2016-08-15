/*
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AlgoException.h"

namespace psa {

const char* AlgoException::InvalidIndex = "The index for {} is not within the range [{}, {}].";
const char* AlgoException::FileOpenRead = "Could not open the '{}' for reading.";

const char* AlgoException::StackUnderflow = "No more elements in the Stack!";

const char* AlgoException::GraphBadFormat = "Bad graph format, expected: {}, actual: {}.";

const char* AlgoException::MatrixZeroDimension =
        "Trying to create a matrix of zero dimension is allowed.";
const char* AlgoException::MatrixDimMismatchForAdd = "Matrix dimensions don't match for addition.";
const char* AlgoException::MatrixDimMismatchForMult =
        "Matrix dimension don't match for multiplication.";

const char* AlgoException::HashTableKeyAlreadyPresent =
        "The key {} is already avilable in the HashTable. Use isExist()!";
const char* AlgoException::HashTableKeyNotFound = "The key {} is not available in the HashTable.";

} // namespace psa
