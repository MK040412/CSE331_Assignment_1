#ifndef LIBRARY_SORT_H
#define LIBRARY_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Library Sort (Gapped Insertion Sort) Implementation
 * 
 * A sorting algorithm that maintains gaps in the array to make insertions more efficient.
 * Based on the paper: "Insertion sort is O(n log n)" by Bender, Farach-Colton, and Mosteiro.
 * 
 * Key characteristics:
 * - Similar to insertion sort but maintains gaps between elements
 * - Periodically rebalances the array to redistribute the gaps
 * - Uses binary search to find insertion positions
 * - Average time complexity: O(n log n)
 * - Worst-case time complexity: O(n²)
 * - Space complexity: O(n) due to the extra space for gaps
 **************************************************************************************/
class LibrarySort {
public:
    /**
     * Execute Library Sort algorithm
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
     * Find insertion position
     * 
     * Uses binary search to find the correct position to insert an element
     * in the gapped array.
     */
    static size_t findPosition(const std::vector<int>& gapped, int key, SortStats& stats);
    
    /**
     * Make room for insertion
     * 
     * Creates space at the insertion position by shifting elements
     * to the right until a gap is found.
     */
    static void makeRoom(std::vector<int>& gapped, size_t pos, SortStats& stats);
    
    /**
     * Rebalance the array
     * 
     * Redistributes elements with uniform gaps when the array becomes too dense.
     * This operation maintains the efficiency of future insertions.
     */
    static void rebalance(std::vector<int>& gapped, size_t elements, double epsilon, SortStats& stats);
};

#endif // LIBRARY_SORT_H
