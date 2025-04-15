#ifndef TIM_SORT_H
#define TIM_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Tim Sort Algorithm
 * 
 * A hybrid stable sorting algorithm combining insertion sort and merge sort.
 * Used in Python, Java, and other programming languages.
 * 
 * Pseudocode:
 * 1. Divide the array into small runs (subarrays)
 * 2. Sort each run using insertion sort
 * 3. Merge the sorted runs using merge sort technique
 * 4. Maintain a stack of pending runs and merge them according to specific criteria
 * 
 * Parameters:
 * - arr: The array to be sorted
 * - MIN_RUN: Minimum size of a run (typically 32)
 * - left/right: Boundaries for sorting operations
 * - l/m/r: Left, middle, and right indices for merge operations
 * - start/end: Start and end indices for run operations
 * - n: Size of the array
 * - stats: Object to track comparisons and swaps during sorting
 **************************************************************************************/
class TimSort {
public:
    /**
     * Execute Tim Sort algorithm
     */
    static SortStats sort(std::vector<int>& arr);
    
    /**
     * Return algorithm name
     */
    static std::string getName();
    
private:
    // Minimum run length
    static const int MIN_RUN = 32;
    static void insertionSort(std::vector<int>& arr, int left, int right, SortStats& stats);
    
    static void merge(std::vector<int>& arr, int l, int m, int r, SortStats& stats);
    
    /**
     * Calculate minimum run length
     */
    static int calcMinRun(int n);
    
    /**
     * Find next run in the array
     */
    static int findNextRun(std::vector<int>& arr, int start, int n, SortStats& stats);
    
    /**
     * Reverse a run if it's in descending order
     */
    static void reverseRun(std::vector<int>& arr, int start, int end, SortStats& stats);
};

#endif 