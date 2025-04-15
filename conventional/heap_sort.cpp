#include "heap_sort.h"

SortStats HeapSort::sort(std::vector<int>& arr) {
    SortStats stats; // Default constructor (initialized to zero)
    
    if (arr.empty() || arr.size() == 1) {
        return stats; // Empty array or array with one element is already sorted
    }
    
    int n = arr.size();
    
    // Build a max heap from the array
    buildHeap(arr, stats);
    
    // Extract elements one by one from the heap to build sorted array
    for (int i = n - 1; i > 0; i--) {
        // Move root (maximum) to the end
        std::swap(arr[0], arr[i]);
        stats.assignments += 3; // swap requires 3 assignment operations
        
        // Reduce heap size and maintain heap property
        heapify(arr, i, 0, stats);
    }
    
    return stats;
}

void HeapSort::heapify(std::vector<int>& arr, int n, int i, SortStats& stats) {
    int largest = i;      // Initialize largest as root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child
    
    // If left child is larger than root
    if (left < n) {
        stats.comparisons++; // Count comparison operation
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    
    // If right child is larger than the largest so far
    if (right < n) {
        stats.comparisons++; // Count comparison operation
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    
    // If largest is not root, swap and recursively heapify
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        stats.assignments += 3; // swap requires 3 assignment operations
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest, stats);
    }
}

void HeapSort::buildHeap(std::vector<int>& arr, SortStats& stats) {
    int n = arr.size();
    
    // Start from the last non-leaf node and heapify all nodes in reverse order
    // Last non-leaf node index: (n/2)-1
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, stats);
    }
}
