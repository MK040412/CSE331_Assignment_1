#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Bubble Sort Algorithm Implementation
 * 
 * Time Complexity: 
 * - Worst case: O(n²) - When array is sorted in reverse order
 * - Average case: O(n²)
 * - Best case: O(n) - When array is already sorted (optimized version)
 * 
 * Space Complexity: O(1) - Requires minimal additional space
 * Stability: Stable
 **************************************************************************************/
class BubbleSort {
public:
    /**
     * Execute Bubble Sort algorithm
     * 
     * @param arr Array to be sorted
     * @return Sorting statistics (comparisons, assignments, memory usage)
     */
    static SortStats sort(std::vector<int>& arr);
};

#endif // BUBBLE_SORT_H
