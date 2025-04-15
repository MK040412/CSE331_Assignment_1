#include "selection_sort.h"

SortStats SelectionSort::sort(std::vector<int>& arr) {
    SortStats stats;
    int n = arr.size();
    
    // Additional memory usage: index variable and temporary variable (8 bytes)
    stats.memoryUsage = sizeof(int) * 2; // min_idx and temporary variable
    
    // Traverse through all array elements
    for (int i = 0; i < n - 1; i++) {
        // Find the minimum element in unsorted portion
        int min_idx = i;
        
        for (int j = i + 1; j < n; j++) {
            stats.comparisons++; // Count comparison operation
            
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Swap the found minimum element with the first element if needed
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            
            stats.assignments += 3; // Swap requires 3 assignment operations
        }
    }
    
    return stats;
}
