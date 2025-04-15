#include "bubble_sort.h"

SortStats BubbleSort::sort(std::vector<int>& arr) {
    SortStats stats;
    int n = arr.size();
    
    // Additional memory usage: temporary variable and flag (8 bytes)
    stats.memoryUsage = sizeof(int) + sizeof(bool); // Temporary variable and swapped flag
    
    // Optimized bubble sort implementation (early termination if no swaps)
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false; // Track if any swaps occurred in this pass
        
        // Last i elements are already sorted, no need to check them
        for (int j = 0; j < n - i - 1; j++) {
            stats.comparisons++; // Comparison of adjacent elements
            
            // Swap if elements are in wrong order
            if (arr[j] > arr[j + 1]) {
                // Swap operation
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                
                stats.assignments += 3; // Swap requires 3 assignment operations
                swapped = true; // Mark that a swap occurred
            }
        }
        
        // If no swaps occurred in this pass, array is already sorted
        if (!swapped) {
            break;
        }
    }
    
    return stats;
}
