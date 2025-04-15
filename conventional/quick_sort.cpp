#include "quick_sort.h"

SortStats QuickSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    if (arr.empty() || arr.size() == 1) {
        return stats; // Empty array or array with one element is already sorted
    }
    
    // Estimate memory usage for recursive call stack (log n depth)
    int n = arr.size();
    int stackDepth = 0;
    int temp = n;
    while (temp > 0) {
        temp /= 2;
        stackDepth++;
    }
    
    // Memory for variables in each recursive call (pivot, indices, etc.)
    stats.memoryUsage = stackDepth * sizeof(int) * 3; // Recursive call stack memory
    
    // Execute quick sort
    quickSort(arr, 0, arr.size() - 1, stats);
    
    return stats;
}

void QuickSort::quickSort(std::vector<int>& arr, int low, int high, SortStats& stats) {
    if (low < high) {
        // Partition the array and get pivot position
        int pivotIndex = partition(arr, low, high, stats);
        
        // Sort left subarray (before pivot)
        quickSort(arr, low, pivotIndex - 1, stats);
        
        // Sort right subarray (after pivot)
        quickSort(arr, pivotIndex + 1, high, stats);
    }
}

int QuickSort::partition(std::vector<int>& arr, int low, int high, SortStats& stats) {
    // Select last element as pivot
    int pivot = arr[high];
    stats.assignments++; // Pivot assignment
    
    // Index of smaller element
    int i = low - 1;
    
    // Process all elements except pivot
    for (int j = low; j < high; j++) {
        stats.comparisons++; // Comparison operation count
        
        if (arr[j] <= pivot) {
            i++;
            
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            
            stats.assignments += 3; // Swap requires 3 assignment operations
        }
    }
    
    // Move pivot to its final position
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    stats.assignments += 3; // Swap requires 3 assignment operations
    
    // Return final position of pivot
    return i + 1;
}
