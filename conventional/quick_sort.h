#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Quick Sort Algorithm Implementation
 * 
 * Time Complexity: 
 * - Worst case: O(n²) - When pivot is always the smallest/largest element
 * - Average case: O(n log n)
 * - Best case: O(n log n) - When pivot is always the middle element
 * 
 * Space Complexity: O(log n) - Recursive call stack
 * Stability: Unstable
 **************************************************************************************/
class QuickSort {
public:
    /**
     * Execute Quick Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);

private:
    /**
     * Recursively partition and sort the array
     */
    static void quickSort(std::vector<int>& arr, int low, int high, SortStats& stats);
    
    /**
     * Partition the array around a pivot
     */
    static int partition(std::vector<int>& arr, int low, int high, SortStats& stats);
};

#endif // QUICK_SORT_H
