#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Merge Sort Algorithm Implementation
 * 
 * Time Complexity: O(n log n) - All cases
 * Space Complexity: O(n) - Requires additional array
 * Stability: Stable
 **************************************************************************************/
class MergeSort {
public:
    /**
     * Execute Merge Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);

private:
    /**
     * Recursively divide and merge the array
     */
    static void mergeSort(std::vector<int>& arr, std::vector<int>& temp, int left, int right, SortStats& stats);
    
    /**
     * Merge two sorted subarrays
     */
    static void merge(std::vector<int>& arr, std::vector<int>& temp, int left, int mid, int right, SortStats& stats);
};

#endif // MERGE_SORT_H
