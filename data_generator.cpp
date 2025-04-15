#include "data_generator.h"
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>

std::vector<int> DataGenerator::generateSortedData(int size, bool ascending) {
    std::vector<int> data(size);
    
    // Generate sorted data from 1 to size
    for (int i = 0; i < size; ++i) {
        data[i] = i + 1;
    }
    
    // Reverse if descending order is requested
    if (!ascending) {
        std::reverse(data.begin(), data.end());
    }
    
    return data;
}

std::vector<int> DataGenerator::generateRandomData(int size, int minVal, int maxVal) {
    std::vector<int> data(size);
    
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    
    // Generate random data
    for (int i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
    
    return data;
}

std::vector<int> DataGenerator::generatePartiallySortedData(int size, double sortedRatio) {
    int sortedSize = static_cast<int>(size * sortedRatio);
    
    // Generate sorted portion
    std::vector<int> data(size);
    for (int i = 0; i < sortedSize; ++i) {
        data[i] = i + 1;
    }
    
    // Generate random portion
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);
    
    for (int i = sortedSize; i < size; ++i) {
        data[i] = dist(gen);
    }
    
    // Partially shuffle the array
    int swapCount = static_cast<int>(size * (1 - sortedRatio) * 0.5);
    std::uniform_int_distribution<int> indexDist(0, size - 1);
    
    for (int i = 0; i < swapCount; ++i) {
        int idx1 = indexDist(gen);
        int idx2 = indexDist(gen);
        std::swap(data[idx1], data[idx2]);
    }
    
    return data;
}

std::vector<int> DataGenerator::generateReverseSortedData(int size) {
    return generateSortedData(size, false);
}

std::map<std::string, std::vector<int>> DataGenerator::generateAllDatasets(const std::vector<int>& sizes) {
    std::map<std::string, std::vector<int>> datasets;
    
    for (int size : sizes) {
        datasets["sorted_asc_" + std::to_string(size)] = generateSortedData(size, true);
        datasets["sorted_desc_" + std::to_string(size)] = generateSortedData(size, false);
        datasets["random_" + std::to_string(size)] = generateRandomData(size);
        datasets["partially_sorted_" + std::to_string(size)] = generatePartiallySortedData(size);
        datasets["reverse_sorted_" + std::to_string(size)] = generateReverseSortedData(size);
    }
    
    return datasets;
}

bool DataGenerator::saveToFile(const std::string& filename, const std::vector<int>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    
    // Save data size
    file << data.size() << std::endl;
    
    // Save data
    for (const auto& value : data) {
        file << value << " ";
    }
    
    file.close();
    return true;
}

bool DataGenerator::loadFromFile(const std::string& filename, std::vector<int>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    
    // Read data size
    int size;
    file >> size;
    
    // Resize data vector
    data.resize(size);
    
    // Read data
    for (int i = 0; i < size; ++i) {
        file >> data[i];
    }
    
    file.close();
    return true;
}
