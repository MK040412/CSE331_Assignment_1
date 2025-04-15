#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Insertion Sort Algorithm Implementation
 * 
 * Time Complexity: 
 * - Worst case: O(n²) - When array is sorted in reverse order
 * - Average case: O(n²)
 * - Best case: O(n) - When array is already sorted
 * 
 * Space Complexity: O(1) - Requires minimal additional space
 * Stability: Stable
 **************************************************************************************/
class InsertionSort {
public:
    /**
     * Execute Insertion Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);
};

#endif // INSERTION_SORT_H
