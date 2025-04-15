#include "library_sort.h"
#include <cmath>
#include <algorithm>

/**************************************************************************************
 * LibrarySort::sort
 * 
 * Main function to execute the Library Sort algorithm on an array.
 * 
 * The algorithm works by:
 * 1. Creating a gapped array with spaces for efficient insertions
 * 2. Inserting elements one by one into their correct positions
 * 3. Periodically rebalancing the array to maintain gaps
 * 4. Compacting the final result back to the original array
 * 
 * Input: Array to be sorted
 * Output: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats LibrarySort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    if (arr.empty()) {
        return stats;
    }
    
    // Initial gap factor (epsilon in the paper)
    const double epsilon = 1.0; // Typically set to 1.0
    
    // Calculate initial capacity based on array size
    size_t capacity = std::ceil(arr.size() * (1.0 + epsilon));
    
    // Create gapped array (using -1 to represent empty spaces)
    std::vector<int> gapped(capacity, -1);
    stats.memoryUsage += gapped.size() * sizeof(int);
    
    // Insert first element
    gapped[0] = arr[0];
    stats.assignments++;
    
    size_t elements = 1; // Count of elements inserted so far
    
    // Insert remaining elements
    for (size_t i = 1; i < arr.size(); i++) {
        // Check if rebalancing is needed
        if (elements >= capacity / 2) {
            // Rebalance to redistribute gaps
            rebalance(gapped, elements, epsilon, stats);
            capacity = gapped.size();
        }
        
        // Find position for insertion
        int key = arr[i];
        size_t j = findPosition(gapped, key, stats);
        
        // Make room for insertion
        makeRoom(gapped, j, stats);
        
        // Insert the element
        gapped[j] = key;
        stats.assignments++;
        
        elements++;
    }
    
    // Copy results back to original array
    size_t index = 0;
    for (size_t i = 0; i < gapped.size(); i++) {
        if (gapped[i] != -1) {
            arr[index++] = gapped[i];
            stats.assignments++;
        }
    }
    
    return stats;
}

/**************************************************************************************
 * LibrarySort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Output: String containing the name "Library Sort"
 **************************************************************************************/
std::string LibrarySort::getName() {
    return "Library Sort";
}

/**************************************************************************************
 * LibrarySort::findPosition
 * 
 * Finds the correct position to insert an element using binary search.
 * Handles the presence of gaps (-1 values) in the array.
 * 
 * Inputs:
 * - Gapped array
 * - Key value to insert
 * - Statistics object to track operations
 * 
 * Output: Index where the key should be inserted
 **************************************************************************************/
size_t LibrarySort::findPosition(const std::vector<int>& gapped, int key, SortStats& stats) {
    int low = 0;
    int high = static_cast<int>(gapped.size()) - 1;
    
    // Binary search to find insertion position
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Skip empty spaces
        while (mid < high && gapped[mid] == -1) mid++;
        while (mid > low && gapped[mid] == -1) mid--;
        
        if (gapped[mid] == -1) {
            // All spaces are empty
            return static_cast<size_t>(low);
        }
        
        stats.comparisons++;
        if (gapped[mid] < key) {
            low = mid + 1;
            // Find next non-empty position
            while (low < high && gapped[low] == -1) low++;
        } else {
            high = mid - 1;
            // Find previous non-empty position
            while (high > low && gapped[high] == -1) high--;
        }
    }
    
    // Find actual insertion position (considering empty spaces)
    size_t pos = static_cast<size_t>(low);
    while (pos < gapped.size() && gapped[pos] != -1 && gapped[pos] < key) {
        stats.comparisons++;
        pos++;
    }
    
    return pos;
}

/**************************************************************************************
 * LibrarySort::makeRoom
 * 
 * Creates space at the insertion position by shifting elements to the right.
 * Stops shifting when an empty space is encountered.
 * 
 * Inputs:
 * - Gapped array
 * - Position where space is needed
 * - Statistics object to track operations
 **************************************************************************************/
void LibrarySort::makeRoom(std::vector<int>& gapped, size_t pos, SortStats& stats) {
    // If position is already empty, no need to make room
    if (pos < gapped.size() && gapped[pos] == -1) {
        return;
    }
    
    // Find next empty space
    size_t nextEmpty = pos;
    while (nextEmpty < gapped.size() && gapped[nextEmpty] != -1) {
        nextEmpty++;
    }
    
    // If no empty space found, use the last position
    if (nextEmpty >= gapped.size()) {
        nextEmpty = gapped.size() - 1;
    }
    
    // Shift elements right to make room
    for (size_t i = nextEmpty; i > pos; i--) {
        gapped[i] = gapped[i-1];
        stats.assignments++;
    }
    
    // Mark insertion position as empty
    gapped[pos] = -1;
}

/**************************************************************************************
 * LibrarySort::rebalance
 * 
 * Redistributes elements in the array with uniform gaps.
 * Called when the array becomes too dense for efficient insertions.
 * 
 * Inputs:
 * - Gapped array
 * - Number of elements currently in the array
 * - Gap factor (epsilon)
 * - Statistics object to track operations
 **************************************************************************************/
void LibrarySort::rebalance(std::vector<int>& gapped, size_t elements, double epsilon, SortStats& stats) {
    // Extract current elements
    std::vector<int> temp;
    temp.reserve(elements);
    
    for (size_t i = 0; i < gapped.size(); i++) {
        if (gapped[i] != -1) {
            temp.push_back(gapped[i]);
        }
    }
    
    // Calculate new capacity
    size_t newCapacity = std::ceil(elements * (1.0 + epsilon)) * 2;
    
    // Create new array with all empty spaces
    gapped.assign(newCapacity, -1);
    stats.memoryUsage = newCapacity * sizeof(int);
    
    // Redistribute elements with uniform gaps
    size_t gap = newCapacity / elements;
    for (size_t i = 0; i < elements; i++) {
        gapped[i * gap] = temp[i];
        stats.assignments++;
    }
}
