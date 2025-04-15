#include "intro_sort.h"
#include <algorithm>
#include <cmath>

/**************************************************************************************
 * IntroSort::sort
 * 
 * Main function to execute the Introsort algorithm on an array.
 * 
 * The algorithm works by:
 * 1. Calculating a recursion depth limit based on array size (2*log2(n))
 * 2. Starting with QuickSort approach
 * 3. Switching to HeapSort if recursion gets too deep
 * 4. Using InsertionSort for small subarrays
 * 
 * Input: Array to be sorted
 * Output: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats IntroSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    int n = arr.size();
    if (n <= 1) {
        return stats;
    }
    
    // Calculate recursion depth limit (2*log(n))
    int depthLimit = 2 * static_cast<int>(std::log2(n));
    
    // Call the main Introsort utility function
    introSortUtil(arr, 0, n - 1, depthLimit, stats);
    
    return stats;
}

/**************************************************************************************
 * IntroSort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Output: String containing the name "Intro Sort"
 **************************************************************************************/
std::string IntroSort::getName() {
    return "Intro Sort";
}

/**************************************************************************************
 * IntroSort::introSortUtil
 * 
 * Core implementation of the Introsort algorithm.
 * 
 * Inputs:
 * - Array to be sorted
 * - Beginning index of the current subarray
 * - Ending index of the current subarray
 * - Recursion depth limit
 * - Statistics object to track operations
 **************************************************************************************/
void IntroSort::introSortUtil(std::vector<int>& arr, int begin, int end, int depthLimit, SortStats& stats) {
    // Calculate size of current subarray
    int size = end - begin + 1;
    
    // Use insertion sort for small arrays (threshold: 16 elements)
    if (size < 16) {
        insertionSort(arr, begin, end, stats);
        return;
    }
    
    // Switch to heap sort if recursion depth limit reached
    if (depthLimit == 0) {
        heapSort(arr, begin, end, stats);
        return;
    }
    
    // Otherwise use quick sort approach
    int pivot = partition(arr, begin, end, stats);
    
    // Recursively sort left subarray
    if (pivot > begin) {
        introSortUtil(arr, begin, pivot - 1, depthLimit - 1, stats);
    }
    
    // Recursively sort right subarray
    if (pivot < end) {
        introSortUtil(arr, pivot + 1, end, depthLimit - 1, stats);
    }
}

/**************************************************************************************
 * IntroSort::insertionSort
 * 
 * Implementation of insertion sort for small subarrays.
 * 
 * Inputs:
 * - Array to be sorted
 * - Beginning index of the subarray
 * - Ending index of the subarray
 * - Statistics object to track operations
 **************************************************************************************/
void IntroSort::insertionSort(std::vector<int>& arr, int begin, int end, SortStats& stats) {
    for (int i = begin + 1; i <= end; i++) {
        int key = arr[i];
        stats.assignments++;
        
        int j = i - 1;
        while (j >= begin) {
            stats.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                stats.assignments++;
                j--;
            } else {
                break;
            }
        }
        
        arr[j + 1] = key;
        stats.assignments++;
    }
}

/**************************************************************************************
 * IntroSort::heapSort
 * 
 * Implementation of heap sort used when recursion depth limit is reached.
 * 
 * Inputs:
 * - Array to be sorted
 * - Beginning index of the subarray
 * - Ending index of the subarray
 * - Statistics object to track operations
 **************************************************************************************/
void IntroSort::heapSort(std::vector<int>& arr, int begin, int end, SortStats& stats) {
    int size = end - begin + 1;
    
    // Build max heap
    for (int i = size / 2 - 1 + begin; i >= begin; i--) {
        heapify(arr, begin, end, i, stats);
    }
    
    // Extract elements from heap one by one
    for (int i = end; i > begin; i--) {
        std::swap(arr[begin], arr[i]);
        stats.assignments += 3; // Swap involves 3 assignment operations
        
        // Restore heap property for the reduced heap
        heapify(arr, begin, i - 1, begin, stats);
    }
}

/**************************************************************************************
 * IntroSort::heapify
 * 
 * Maintains the heap property for the heap sort algorithm.
 * 
 * Inputs:
 * - Array containing the heap
 * - Beginning index of the heap
 * - Ending index of the heap
 * - Current node index to heapify
 * - Statistics object to track operations
 **************************************************************************************/
void IntroSort::heapify(std::vector<int>& arr, int begin, int end, int i, SortStats& stats) {
    int largest = i;
    int left = 2 * (i - begin) + 1 + begin;
    int right = 2 * (i - begin) + 2 + begin;
    
    // Check if left child is larger than root
    if (left <= end) {
        stats.comparisons++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    
    // Check if right child is larger than current largest
    if (right <= end) {
        stats.comparisons++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    
    // If largest is not the root, swap and continue heapifying
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        stats.assignments += 3; // Swap involves 3 assignment operations
        
        heapify(arr, begin, end, largest, stats);
    }
}

/**************************************************************************************
 * IntroSort::partition
 * 
 * Partitions the array for the QuickSort portion of Introsort.
 * Uses median-of-three pivot selection for better performance.
 * 
 * Inputs:
 * - Array to be partitioned
 * - Beginning index of the subarray
 * - Ending index of the subarray
 * - Statistics object to track operations
 * 
 * Output: Index of the pivot element after partitioning
 **************************************************************************************/
int IntroSort::partition(std::vector<int>& arr, int begin, int end, SortStats& stats) {
    // Select pivot using median-of-three method
    int mid = begin + (end - begin) / 2;
    int pivotIndex = medianOfThree(arr, begin, mid, end, stats);
    int pivotValue = arr[pivotIndex];
    
    // Move pivot to the end
    std::swap(arr[pivotIndex], arr[end]);
    stats.assignments += 3;
    
    int i = begin - 1;
    
    // Partition process
    for (int j = begin; j < end; j++) {
        stats.comparisons++;
        if (arr[j] <= pivotValue) {
            i++;
            std::swap(arr[i], arr[j]);
            stats.assignments += 3;
        }
    }
    
    // Move pivot to its final position
    std::swap(arr[i + 1], arr[end]);
    stats.assignments += 3;
    
    return i + 1;
}

/**************************************************************************************
 * IntroSort::medianOfThree
 * 
 * Selects the median of three elements as the pivot for partitioning.
 * This helps avoid worst-case performance in QuickSort.
 * 
 * Inputs:
 * - Array containing the elements
 * - Index of the first element
 * - Index of the middle element
 * - Index of the last element
 * - Statistics object to track operations
 * 
 * Output: Index of the median element
 **************************************************************************************/
int IntroSort::medianOfThree(std::vector<int>& arr, int begin, int mid, int end, SortStats& stats) {
    // Compare the three values to find the median
    stats.comparisons += 3;
    
    if (arr[begin] < arr[mid]) {
        if (arr[mid] < arr[end]) {
            return mid; // begin < mid < end
        } else if (arr[begin] < arr[end]) {
            return end; // begin < end < mid
        } else {
            return begin; // end < begin < mid
        }
    } else {
        if (arr[begin] < arr[end]) {
            return begin; // mid < begin < end
        } else if (arr[mid] < arr[end]) {
            return end; // mid < end < begin
        } else {
            return mid; // end < mid < begin
        }
    }
}
