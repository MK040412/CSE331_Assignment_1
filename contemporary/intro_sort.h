#ifndef INTRO_SORT_H
#define INTRO_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Introsort Implementation
 * 
 * A hybrid sorting algorithm that combines QuickSort, HeapSort, and InsertionSort
 * to guarantee O(n log n) worst-case time complexity while maintaining good
 * average-case performance.
 * 
 * Key characteristics:
 * - Begins with QuickSort for good average-case performance
 * - Switches to HeapSort when recursion depth exceeds a limit to avoid QuickSort's worst case
 * - Uses InsertionSort for small arrays to improve efficiency
 * - Worst-case time complexity: O(n log n)
 * - Average time complexity: O(n log n)
 * - Best-case time complexity: O(n log n)
 * - Space complexity: O(log n) for recursion stack
 **************************************************************************************/
class IntroSort {
public:
    /**
     * Execute Introsort algorithm
     * 
     * Takes an array to be sorted and returns sorting statistics.
     */
    static SortStats sort(std::vector<int>& arr);
    
    /**
     * Return algorithm name
     * 
     * Returns the name of this sorting algorithm.
     */
    static std::string getName();
    
private:
    /**
     * Main Introsort implementation
     * 
     * Recursively sorts the array using a combination of algorithms based on
     * array size and recursion depth.
     */
    static void introSortUtil(std::vector<int>& arr, int begin, int end, int depthLimit, SortStats& stats);
    
    /**
     * Insertion sort implementation
     * 
     * Used for small arrays or as the final step of Introsort.
     */
    static void insertionSort(std::vector<int>& arr, int begin, int end, SortStats& stats);
    
    /**
     * Heap sort implementation
     * 
     * Used when recursion depth exceeds the limit to avoid QuickSort's worst case.
     */
    static void heapSort(std::vector<int>& arr, int begin, int end, SortStats& stats);
    
    /**
     * Heapify function for heap sort
     * 
     * Maintains the heap property for the heap sort algorithm.
     */
    static void heapify(std::vector<int>& arr, int begin, int end, int i, SortStats& stats);
    
    /**
     * Partition function for quick sort
     * 
     * Divides the array around a pivot element.
     */
    static int partition(std::vector<int>& arr, int begin, int end, SortStats& stats);
    
    /**
     * Median-of-three pivot selection
     * 
     * Chooses a good pivot to improve QuickSort performance.
     */
    static int medianOfThree(std::vector<int>& arr, int begin, int mid, int end, SortStats& stats);
};

#endif // INTRO_SORT_H
