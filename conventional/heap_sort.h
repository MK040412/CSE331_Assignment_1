#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <vector>
#include "sorting_algorithms.h" // Include for SortStats structure

/**************************************************************************************
 * Heap Sort Algorithm Implementation
 * 
 * Time Complexity: O(n log n) - All cases
 * Space Complexity: O(1) - No additional space required
 * Stability: Unstable
 **************************************************************************************/
class HeapSort {
public:
    /**
     * Execute Heap Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);

private:
    /**
     * Maintain max heap property
     */
    static void heapify(std::vector<int>& arr, int n, int i, SortStats& stats);
    
    /**
     * Build a max heap from an array
     */
    static void buildHeap(std::vector<int>& arr, SortStats& stats);
};

#endif // HEAP_SORT_H
