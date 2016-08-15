/**
 * Header file sorting algorithms
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_SORTING_H
#define PSA_SORTING_H

#include <vector>

namespace psa {

void insertionSort(std::vector<int>& a);

void mergeSort(std::vector<int>& a);

void heapSort(std::vector<int>& a);

std::ptrdiff_t quickSort(int* p, int* r);
std::ptrdiff_t quickSort(std::vector<int>& a);

std::ptrdiff_t randomizedQuickSort(int* p, int* r);
std::ptrdiff_t randomizedQuickSort(std::vector<int>& a);

}

#endif // PSA_SORTING_H
