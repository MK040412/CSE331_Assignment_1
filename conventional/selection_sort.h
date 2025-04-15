#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Selection Sort Algorithm Implementation
 * 
 * Time Complexity: 
 * - Worst case: O(n²) - All cases
 * - Average case: O(n²)
 * - Best case: O(n²) - Even with sorted array, all comparisons are performed
 * 
 * Space Complexity: O(1) - Requires minimal additional space
 * Stability: Unstable - May change the relative order of equal elements
 **************************************************************************************/
class SelectionSort {
public:
    /**
     * Execute Selection Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);
};

#endif // SELECTION_SORT_H
