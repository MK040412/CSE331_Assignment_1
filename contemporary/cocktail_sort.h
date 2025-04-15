#ifndef COCKTAIL_SORT_H
#define COCKTAIL_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Cocktail Sort (Cocktail Shaker Sort) Implementation
 * 
 * A variation of Bubble Sort that traverses the array in both directions alternately.
 * This bidirectional approach can lead to faster convergence for some data sets
 * compared to the standard Bubble Sort.
 * 
 * Key characteristics:
 * - Stable sorting algorithm
 * - In-place sorting with O(1) extra space
 * - Average and worst-case time complexity: O(n²)
 * - Best-case time complexity: O(n) when array is already sorted
 **************************************************************************************/
class CocktailSort {
public:
    /**
     * Execute Cocktail Sort algorithm
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
};

#endif // COCKTAIL_SORT_H
