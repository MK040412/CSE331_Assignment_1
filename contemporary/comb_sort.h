#ifndef COMB_SORT_H
#define COMB_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Comb Sort Implementation
 * 
 * An improved version of Bubble Sort that addresses the "turtle" problem
 * (small values near the end of the array that slow down the sorting process).
 * 
 * Key characteristics:
 * - Uses a gap sequence that starts large and shrinks with each iteration
 * - The gap reduces by a shrink factor (typically 1.3) until it reaches 1
 * - When gap = 1, the algorithm works like Bubble Sort
 * - Average time complexity: O(n² / 2^p) where p is the number of increments
 * - Worst-case time complexity: O(n²)
 * - Best-case time complexity: O(n log n)
 **************************************************************************************/
class CombSort {
public:
    /**
     * Execute Comb Sort algorithm
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
     * Calculate next gap value
     * 
     * Reduces the current gap by a shrink factor (typically 1.3).
     * Ensures the gap never goes below 1.
     */
    static int getNextGap(int gap);
};

#endif // COMB_SORT_H
