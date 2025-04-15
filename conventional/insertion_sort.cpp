#include "insertion_sort.h"

SortStats InsertionSort::sort(std::vector<int>& arr) {
    SortStats stats;
    int n = arr.size();
    
    // Additional memory usage: key variable (4 bytes) and j variable (4 bytes)
    stats.memoryUsage = sizeof(int) * 2;
    
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        stats.assignments++; // key assignment
        
        int j = i - 1;
        
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            stats.comparisons++; // arr[j] > key comparison
            
            arr[j + 1] = arr[j];
            stats.assignments++; // arr[j+1] assignment
            
            j--;
        }
        
        // Last comparison (while loop exit condition)
        if (j >= 0) {
            stats.comparisons++;
        }
        
        arr[j + 1] = key;
        stats.assignments++; // arr[j+1] = key assignment
    }
    
    return stats;
}
