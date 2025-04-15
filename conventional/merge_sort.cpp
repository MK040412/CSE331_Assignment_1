#include "merge_sort.h"

SortStats MergeSort::sort(std::vector<int>& arr) {
    SortStats stats; // Default constructor (initialized to zero)
    
    if (arr.empty() || arr.size() == 1) {
        return stats; // Empty array or array with one element is already sorted
    }
    
    // Allocate temporary array (used during merging)
    std::vector<int> temp(arr.size());
    stats.memoryUsage += arr.size() * sizeof(int); // Add memory usage for temporary array
    
    // Execute merge sort
    mergeSort(arr, temp, 0, arr.size() - 1, stats);
    
    return stats;
}

void MergeSort::mergeSort(std::vector<int>& arr, std::vector<int>& temp, int left, int right, SortStats& stats) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Calculate midpoint (prevents overflow)
        
        // Sort left subarray
        mergeSort(arr, temp, left, mid, stats);
        
        // Sort right subarray
        mergeSort(arr, temp, mid + 1, right, stats);
        
        // Merge the sorted subarrays
        merge(arr, temp, left, mid, right, stats);
    }
}

void MergeSort::merge(std::vector<int>& arr, std::vector<int>& temp, int left, int mid, int right, SortStats& stats) {
    // Left subarray: arr[left..mid]
    // Right subarray: arr[mid+1..right]
    
    // Copy to temporary array
    for (int i = left; i <= right; i++) {
        temp[i] = arr[i];
        stats.assignments++; // Count assignment operation
    }
    
    int i = left;      // Starting index of left subarray
    int j = mid + 1;   // Starting index of right subarray
    int k = left;      // Current index in result array
    
    // Compare and merge the two subarrays, placing smaller elements first
    while (i <= mid && j <= right) {
        stats.comparisons++; // Count comparison operation
        
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = temp[j];
            j++;
        }
        
        stats.assignments++; // Count assignment operation
        k++;
    }
    
    // Copy remaining elements from left subarray if any
    while (i <= mid) {
        arr[k] = temp[i];
        stats.assignments++; // Count assignment operation
        i++;
        k++;
    }
    
    // Copy remaining elements from right subarray if any
    // (Actually could be skipped as they're already in correct position, but included for clarity)
    while (j <= right) {
        arr[k] = temp[j];
        stats.assignments++; // Count assignment operation
        j++;
        k++;
    }
}
