#include "tim_sort.h"
#include <algorithm>
#include <stack>

/**************************************************************************************
 * TimSort::sort
 * 
 * Main function to execute the Tim Sort algorithm on an array.
 * 
 * Steps:
 * 1. Calculate minimum run length
 * 2. Divide array into runs and sort each run using insertion sort
 * 3. Merge runs using bottom-up merge sort approach
 * 
 * Parameters:
 * - arr: The array to be sorted
 * 
 * Returns:
 * - SortStats: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats TimSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    int n = arr.size();
    if (n <= 1) {
        return stats;
    }
    
    int minRun = calcMinRun(n);
    
    // Sort individual runs using insertion sort
    for (int i = 0; i < n; i += minRun) {
        int end = std::min(i + minRun - 1, n - 1);
        insertionSort(arr, i, end, stats);
    }
    
    // Merge runs using bottom-up merge sort approach
    for (int size = minRun; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            
            if (mid < right) {
                merge(arr, left, mid, right, stats);
            }
        }
    }
    
    return stats;
}

/**************************************************************************************
 * TimSort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Returns:
 * - string: Name of the algorithm ("Tim Sort")
 **************************************************************************************/
std::string TimSort::getName() {
    return "Tim Sort";
}

/**************************************************************************************
 * TimSort::insertionSort
 * 
 * Performs insertion sort on a segment of the array.
 * 
 * Parameters:
 * - arr: The array to be sorted
 * - left: Starting index of the segment
 * - right: Ending index of the segment
 * - stats: Object to track comparisons and assignments
 **************************************************************************************/
void TimSort::insertionSort(std::vector<int>& arr, int left, int right, SortStats& stats) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        stats.assignments++;
        
        int j = i - 1;
        while (j >= left) {
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
 * TimSort::merge
 * 
 * Merges two adjacent sorted segments of the array.
 * 
 * Parameters:
 * - arr: The array containing segments to merge
 * - l: Left boundary of first segment
 * - m: Middle point (end of first segment)
 * - r: Right boundary of second segment
 * - stats: Object to track comparisons and assignments
 **************************************************************************************/
void TimSort::merge(std::vector<int>& arr, int l, int m, int r, SortStats& stats) {
    // Calculate sizes of left and right subarrays
    int len1 = m - l + 1;
    int len2 = r - m;
    
    // Create temporary arrays
    std::vector<int> left(len1);
    std::vector<int> right(len2);
    stats.memoryUsage += (len1 + len2) * sizeof(int);
    
    // Copy data to temporary arrays
    for (int i = 0; i < len1; i++) {
        left[i] = arr[l + i];
        stats.assignments++;
    }
    
    for (int i = 0; i < len2; i++) {
        right[i] = arr[m + 1 + i];
        stats.assignments++;
    }
    
    // Merge the temporary arrays back
    int i = 0, j = 0, k = l;
    
    while (i < len1 && j < len2) {
        stats.comparisons++;
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        stats.assignments++;
        k++;
    }
    
    // Copy remaining elements
    while (i < len1) {
        arr[k] = left[i];
        stats.assignments++;
        i++;
        k++;
    }
    
    while (j < len2) {
        arr[k] = right[j];
        stats.assignments++;
        j++;
        k++;
    }
}

/**************************************************************************************
 * TimSort::calcMinRun
 * 
 * Calculates the minimum length of a run.
 * This is an important parameter for Tim Sort efficiency.
 * 
 * Parameters:
 * - n: Size of the array
 * 
 * Returns:
 * - int: Minimum run length
 **************************************************************************************/
int TimSort::calcMinRun(int n) {
    int r = 0;
    while (n >= MIN_RUN) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

/**************************************************************************************
 * TimSort::findNextRun
 * 
 * Identifies the next natural run in the array.
 * A run is a sequence of elements that is already sorted (ascending or descending).
 * 
 * Parameters:
 * - arr: The array to search for runs
 * - start: Starting index to search from
 * - n: Size of the array
 * - stats: Object to track comparisons
 * 
 * Returns:
 * - int: Ending index of the identified run
 **************************************************************************************/
int TimSort::findNextRun(std::vector<int>& arr, int start, int n, SortStats& stats) {
    if (start >= n - 1) {
        return start;
    }
    
    // Check for ascending run
    if (arr[start] <= arr[start + 1]) {
        stats.comparisons++;
        int i = start + 1;
        while (i < n - 1 && arr[i] <= arr[i + 1]) {
            stats.comparisons++;
            i++;
        }
        return i;
    } 
    // Check for descending run
    else {
        stats.comparisons++;
        int i = start + 1;
        while (i < n - 1 && arr[i] > arr[i + 1]) {
            stats.comparisons++;
            i++;
        }
        // Reverse descending run to make it ascending
        reverseRun(arr, start, i, stats);
        return i;
    }
}

/**************************************************************************************
 * TimSort::reverseRun
 * 
 * Reverses a segment of the array (typically to convert a descending run to ascending).
 * 
 * Parameters:
 * - arr: The array containing the segment to reverse
 * - start: Starting index of the segment
 * - end: Ending index of the segment
 * - stats: Object to track assignments
 **************************************************************************************/
void TimSort::reverseRun(std::vector<int>& arr, int start, int end, SortStats& stats) {
    while (start < end) {
        std::swap(arr[start], arr[end]);
        stats.assignments += 3; // Swap involves 3 assignment operations
        start++;
        end--;
    }
}
