#include "comb_sort.h"

/**************************************************************************************
 * CombSort::sort
 * 
 * Main function to execute the Comb Sort algorithm on an array.
 * 
 * The algorithm works by:
 * 1. Starting with a large gap and comparing elements that are gap positions apart
 * 2. Gradually reducing the gap using a shrink factor
 * 3. When gap = 1, performing a final pass similar to Bubble Sort
 * 
 * Input: Array to be sorted
 * Output: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats CombSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    int n = arr.size();
    if (n <= 1) {
        return stats;
    }
    
    // Initialize gap
    int gap = n;
    
    // Flag to track if any swaps occurred
    bool swapped = true;
    
    // Continue until gap = 1 and no swaps occur
    while (gap > 1 || swapped) {
        // Calculate next gap
        gap = getNextGap(gap);
        
        // Reset swap flag for this iteration
        swapped = false;
        
        // Compare and swap elements that are gap positions apart
        for (int i = 0; i < n - gap; i++) {
            stats.comparisons++;
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                stats.assignments += 3; // Swap involves 3 assignment operations
                swapped = true;
            }
        }
    }
    
    return stats;
}

/**************************************************************************************
 * CombSort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Output: String containing the name "Comb Sort"
 **************************************************************************************/
std::string CombSort::getName() {
    return "Comb Sort";
}

/**************************************************************************************
 * CombSort::getNextGap
 * 
 * Calculates the next gap value by dividing the current gap by the shrink factor.
 * The shrink factor is typically 1.3, which has been found empirically to be effective.
 * 
 * Input: Current gap value
 * Output: Next gap value (minimum 1)
 **************************************************************************************/
int CombSort::getNextGap(int gap) {
    // Shrink factor (1.3 is commonly used)
    const double shrink = 1.3;
    
    // Calculate next gap
    gap = static_cast<int>(gap / shrink);
    
    // Ensure gap doesn't go below 1
    return (gap < 1) ? 1 : gap;
}
