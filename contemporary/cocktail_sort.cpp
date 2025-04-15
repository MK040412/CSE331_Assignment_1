#include "cocktail_sort.h"

/**************************************************************************************
 * CocktailSort::sort
 * 
 * Main function to execute the Cocktail Sort algorithm on an array.
 * 
 * The algorithm works by:
 * 1. Moving from left to right, bubbling the largest element to the end
 * 2. Moving from right to left, bubbling the smallest element to the beginning
 * 3. Repeating until no swaps are needed
 * 
 * Input: Array to be sorted
 * Output: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats CocktailSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    int n = arr.size();
    if (n <= 1) {
        return stats;
    }
    
    bool swapped = true;
    int start = 0;
    int end = n - 1;
    
    while (swapped) {
        // Forward pass: move from left to right, pushing larger elements to the end
        swapped = false;
        for (int i = start; i < end; i++) {
            stats.comparisons++;
            if (arr[i] > arr[i + 1]) {
                // Manual swap instead of std::swap for consistent tracking
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                stats.assignments += 3; // Swap involves 3 assignment operations
                swapped = true;
            }
        }
        
        // If no swaps occurred, array is sorted
        if (!swapped) {
            break;
        }
        
        // Decrease end boundary as the largest element is now at the end
        end--;
        
        // Backward pass: move from right to left, pushing smaller elements to the beginning
        swapped = false;
        for (int i = end - 1; i >= start; i--) {
            stats.comparisons++;
            if (arr[i] > arr[i + 1]) {
                // Manual swap instead of std::swap for consistent tracking
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                stats.assignments += 3; // Swap involves 3 assignment operations
                swapped = true;
            }
        }
        
        // Increase start boundary as the smallest element is now at the beginning
        start++;
    }
    
    return stats;
}

/**************************************************************************************
 * CocktailSort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Output: String containing the name "Cocktail Sort"
 **************************************************************************************/
std::string CocktailSort::getName() {
    return "Cocktail Sort";
}
