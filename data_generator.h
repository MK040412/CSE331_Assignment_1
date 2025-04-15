#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include <vector>
#include <string>
#include <map>

/**************************************************************************************
 * Data Generator Class
 * 
 * A utility class for generating various test datasets for sorting algorithm evaluation.
 * Provides methods to create sorted, random, partially sorted, and reverse sorted data.
 * Also includes functionality to save and load datasets from files.
 **************************************************************************************/
class DataGenerator {
public:
    /**
     * Generate sorted data in ascending or descending order
     */
    static std::vector<int> generateSortedData(int size, bool ascending = true);
    
    /**
     * Generate random data within specified range
     */
    static std::vector<int> generateRandomData(int size, int minVal = 1, int maxVal = 1000000);
    
    /**
     * Generate partially sorted data with specified ratio of sorted elements
     */
    static std::vector<int> generatePartiallySortedData(int size, double sortedRatio = 0.7);
    
    /**
     * Generate reverse sorted data (descending order)
     */
    static std::vector<int> generateReverseSortedData(int size);
    
    /**
     * Generate all types of datasets for given sizes
     */
    static std::map<std::string, std::vector<int>> generateAllDatasets(const std::vector<int>& sizes);
    
    /**
     * Save data to a file
     */
    static bool saveToFile(const std::string& filename, const std::vector<int>& data);
    
    /**
     * Load data from a file
     */
    static bool loadFromFile(const std::string& filename, std::vector<int>& data);
};

#endif // DATA_GENERATOR_H
