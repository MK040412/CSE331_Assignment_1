#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>
#include <string>

/**************************************************************************************
 * SortStats Structure
 * 
 * A structure to store performance statistics of sorting algorithms.
 * Tracks the number of comparisons, assignments, and memory usage during sorting.
 * 
 * Fields:
 * - comparisons: Number of comparison operations
 * - assignments: Number of assignment operations
 * - memoryUsage: Memory usage in bytes
 * 
 * Methods:
 * - reset(): Resets all statistics to zero
 * - merge(): Combines statistics with another SortStats object
 **************************************************************************************/
struct SortStats {
    long long comparisons;    // Number of comparison operations
    long long assignments;    // Number of assignment operations
    size_t memoryUsage;       // Memory usage in bytes
    
    // Default constructor
    SortStats() : comparisons(0), assignments(0), memoryUsage(0) {}
    
    // Reset statistics method
    void reset() {
        comparisons = 0;
        assignments = 0;
        memoryUsage = 0;
    }
    
    // Merge with another statistics object
    void merge(const SortStats& other) {
        comparisons += other.comparisons;
        assignments += other.assignments;
        if (other.memoryUsage > memoryUsage) {
            memoryUsage = other.memoryUsage;
        }
    }
};

/**************************************************************************************
 * SortingAlgorithm Interface
 * 
 * Common interface that all sorting algorithm classes should follow.
 * Defines the contract for sorting algorithms with methods for execution and identification.
 * 
 * Methods:
 * - sort(): Execute the sorting algorithm on the provided array
 * - getName(): Return the name of the algorithm
 **************************************************************************************/
class SortingAlgorithm {
public:
    virtual ~SortingAlgorithm() = default;
    
    /**
     * Execute the sorting algorithm
     * 
     * @param arr Array to be sorted
     * @return Sorting statistics
     */
    virtual SortStats sort(std::vector<int>& arr) = 0;
    
    /**
     * Return algorithm name
     * 
     * @return Name of the algorithm
     */
    virtual std::string getName() const = 0;
};

/**************************************************************************************
 * Utility Function
 * 
 * Helper function to verify if an array is properly sorted.
 * 
 * Parameters:
 * - arr: The array to be verified
 * 
 * Returns:
 * - true if the array is sorted in ascending order
 * - false otherwise
 **************************************************************************************/
inline bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

#endif // SORTING_ALGORITHMS_H
