/**************************************************************************************
 * Sorting Algorithm Performance Evaluation Framework
 * 
 * This program evaluates and compares the performance of various sorting algorithms
 * across different types of datasets. It measures execution time, number of comparisons,
 * assignments, memory usage, and stability characteristics.
 * 
 * Features:
 * - Generates test datasets of various sizes and patterns (sorted, random, etc.)
 * - Tests both conventional and contemporary sorting algorithms
 * - Measures performance metrics including time, operations, and memory usage
 * - Verifies sorting correctness and algorithm stability
 * - Exports results to CSV for further analysis
 * - Provides comparative analysis between algorithms
 * 
 * Algorithms evaluated:
 * - Conventional: Bubble, Insertion, Selection, Quick, Merge, Heap
 * - Contemporary: Library, Tim, Cocktail, Comb, Tournament, Intro
 * 
 * Dataset types:
 * - Sorted (ascending)
 * - Sorted (descending)
 * - Random
 * - Partially sorted
 * - Reverse sorted
 **************************************************************************************/

 #include <fstream>
 #include <iostream>
 #include <vector>
 #include <string>
 #include <filesystem>
 #include <chrono>
 #include <iomanip>
 #include <cmath>
 #include <map>
 #include <algorithm>
 #include <numeric>
 #include <functional>
 #include <filesystem>
 #include "data_generator.h"
 
 // Conventional Sort 
 #include "conventional/bubble_sort.h"
 #include "conventional/insertion_sort.h"
 #include "conventional/selection_sort.h"
 #include "conventional/quick_sort.h"
 #include "conventional/merge_sort.h"
 #include "conventional/heap_sort.h"
 
 // Contemporary Sort
 #include "contemporary/library_sort.h"
 #include "contemporary/tim_sort.h"
 #include "contemporary/cocktail_sort.h"
 #include "contemporary/comb_sort.h"
 #include "contemporary/tournament_sort.h"
 #include "contemporary/intro_sort.h"
 
 
 namespace fs = std::filesystem;
 using namespace std;
 using namespace chrono;
 
 // Test iterations (minimum 10)
 const int TEST_ITERATIONS = 10;
 
 // Algorithm stability information (true: stable, false: unstable)
 const map<string, bool> ALGORITHM_STABILITY = {
     {"Bubble Sort", true},
     {"Insertion Sort", true},
     {"Selection Sort", false},
     {"Quick Sort", false},
     {"Merge Sort", true},
     {"Heap Sort", false},
     {"Library Sort", true},
     {"Tim Sort", true},
     {"Cocktail Sort", true},
     {"Comb Sort", false},
     {"Tournament Sort", false},
     {"Intro Sort", false}
 };
 
 // Algorithm theoretical time complexity information
 const map<string, map<string, string>> ALGORITHM_COMPLEXITY = {
     {"Bubble Sort", {{"Best", "O(n)"}, {"Average", "O(n²)"}, {"Worst", "O(n²)"}}},
     {"Insertion Sort", {{"Best", "O(n)"}, {"Average", "O(n²)"}, {"Worst", "O(n²)"}}},
     {"Selection Sort", {{"Best", "O(n²)"}, {"Average", "O(n²)"}, {"Worst", "O(n²)"}}},
     {"Quick Sort", {{"Best", "O(n log n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n²)"}}},
     {"Merge Sort", {{"Best", "O(n log n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n log n)"}}},
     {"Heap Sort", {{"Best", "O(n log n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n log n)"}}},
     {"Library Sort", {{"Best", "O(n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n²)"}}},
     {"Tim Sort", {{"Best", "O(n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n log n)"}}},
     {"Cocktail Sort", {{"Best", "O(n)"}, {"Average", "O(n²)"}, {"Worst", "O(n²)"}}},
     {"Comb Sort", {{"Best", "O(n log n)"}, {"Average", "O(n²)"}, {"Worst", "O(n²)"}}},
     {"Tournament Sort", {{"Best", "O(n log n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n log n)"}}},
     {"Intro Sort", {{"Best", "O(n log n)"}, {"Average", "O(n log n)"}, {"Worst", "O(n log n)"}}},
 };
 
 /**************************************************************************************
  * SortResult Structure
  * 
  * Stores the results of a sorting algorithm test run.
  * Contains performance metrics and test information.
  **************************************************************************************/
 struct SortResult {
     string algorithmName;
     string datasetName;
     int dataSize;
     vector<long long> executionTimes; // in milliseconds
     double avgExecutionTime;
     long long comparisons;
     long long assignments;
     long long memoryUsage;
     bool isStable;
     bool isSorted;
     bool measuredStability; // Actual measured stability
 };
 
 /**************************************************************************************
  * StabilityTestItem Structure
  * 
  * Used for testing the stability of sorting algorithms.
  * Contains a key for sorting and original position for tracking.
  **************************************************************************************/
 struct StabilityTestItem {
     int key;        // Sort key
     int originalPos; // Original position
     
     // Comparison operators (compare by key only)
     bool operator<(const StabilityTestItem& other) const {
         return key < other.key;
     }
     
     bool operator==(const StabilityTestItem& other) const {
         return key == other.key;
     }
 };
 
 // Log output function (flush immediately for real-time output)
 void log(const string& message) {
     cout << message << endl;
     cout.flush(); // Flush output buffer immediately
 }
 
 // Sorting verification function
 bool verifySorting(const vector<int>& arr) {
     for (size_t i = 1; i < arr.size(); i++) {
         if (arr[i] < arr[i-1]) {
             return false;
         }
     }
     return true;
 }
 
 // Generate stability test data
 vector<StabilityTestItem> generateStabilityTestData(int size) {
     vector<StabilityTestItem> data;
     data.reserve(size);
     
     // Generate data with duplicate keys (keys limited to 1-10 range)
     for (int i = 0; i < size; i++) {
         StabilityTestItem item;
         item.key = rand() % 10 + 1; // Key value between 1-10
         item.originalPos = i;       // Store original position
         data.push_back(item);
     }
     
     return data;
 }
 // Test algorithm stability
bool testStability(const string& algorithmName, int dataSize) {
    log("Stability test: " + algorithmName);
    
    // Generate stability test data
    vector<StabilityTestItem> testData = generateStabilityTestData(dataSize);
    
    // Copy original data
    vector<StabilityTestItem> originalData = testData;
    
    // Apply sorting algorithm (call appropriate algorithm)
    if (algorithmName == "Bubble Sort") {
        // Bubble sort wrapper function
        auto bubbleSortWrapper = [](vector<StabilityTestItem>& items) {
            for (size_t i = 0; i < items.size(); i++) {
                for (size_t j = 0; j < items.size() - i - 1; j++) {
                    if (items[j].key > items[j + 1].key) {
                        // Stable swap method
                        StabilityTestItem temp = items[j];
                        items[j] = items[j + 1];
                        items[j + 1] = temp;
                    }
                }
            }
        };
        bubbleSortWrapper(testData);
    } 
    else if (algorithmName == "Insertion Sort") {
        // Insertion sort wrapper function
        auto insertionSortWrapper = [](vector<StabilityTestItem>& items) {
            for (size_t i = 1; i < items.size(); i++) {
                StabilityTestItem key = items[i];
                int j = i - 1;
                while (j >= 0 && items[j].key > key.key) {
                    items[j + 1] = items[j];
                    j--;
                }
                items[j + 1] = key;
            }
        };
        insertionSortWrapper(testData);
    }
    else if (algorithmName == "Selection Sort") {
        // Selection sort wrapper function - inherently unstable
        auto selectionSortWrapper = [](vector<StabilityTestItem>& items) {
            for (size_t i = 0; i < items.size(); i++) {
                size_t min_idx = i;
                for (size_t j = i + 1; j < items.size(); j++) {
                    if (items[j].key < items[min_idx].key) {
                        min_idx = j;
                    }
                }
                // This swap can break stability by moving items with equal keys
                if (min_idx != i) {
                    swap(items[i], items[min_idx]);
                }
            }
        };
        selectionSortWrapper(testData);
    }
    else if (algorithmName == "Quick Sort") {
        // Quick sort wrapper function - inherently unstable
        std::function<void(vector<StabilityTestItem>&, int, int)> quickSort;
        quickSort = [&quickSort](vector<StabilityTestItem>& items, int low, int high) {
                if (low < high) {
                    // Partition
                    StabilityTestItem pivot = items[high];
                    int i = low - 1;
                    for (int j = low; j <= high - 1; j++) {
                        if (items[j].key <= pivot.key) {
                            i++;
                            // This swap can break stability
                            swap(items[i], items[j]);
                        }
                    }
                    // This swap can also break stability
                    swap(items[i + 1], items[high]);
                    int pi = i + 1;
                    
                    // Recursive calls
                    quickSort(items, low, pi - 1);
                    quickSort(items, pi + 1, high);
                }
            };
        quickSort(testData, 0, testData.size() - 1);
    }
    else if (algorithmName == "Merge Sort") {
        // Merge sort wrapper function - stable
        function<void(vector<StabilityTestItem>&, int, int)> mergeSort;
        mergeSort = [&mergeSort](vector<StabilityTestItem>& items, int left, int right) {
                if (left < right) {
                    int mid = left + (right - left) / 2;
                    mergeSort(items, left, mid);
                    mergeSort(items, mid + 1, right);
                    
                    // Merge
                    vector<StabilityTestItem> temp(right - left + 1);
                    int i = left, j = mid + 1, k = 0;
                    
                    while (i <= mid && j <= right) {
                        // Stable comparison: use <= to maintain order of equal keys
                        if (items[i].key <= items[j].key) {
                            temp[k++] = items[i++];
                        } else {
                            temp[k++] = items[j++];
                        }
                    }
                    
                    while (i <= mid) {
                        temp[k++] = items[i++];
                    }
                    
                    while (j <= right) {
                        temp[k++] = items[j++];
                    }
                    
                    for (i = 0; i < k; i++) {
                        items[left + i] = temp[i];
                    }
                }
            };
        mergeSort(testData, 0, testData.size() - 1);
    }
    else if (algorithmName == "Heap Sort") {
        // Heap sort wrapper function - inherently unstable
        function<void(vector<StabilityTestItem>&, int, int)> heapify;
        heapify = [&heapify](vector<StabilityTestItem>& items, int n, int i) {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            
            if (left < n && items[left].key > items[largest].key)
                largest = left;
            
            if (right < n && items[right].key > items[largest].key)
                largest = right;
            
            if (largest != i) {
                // This swap can break stability
                swap(items[i], items[largest]);
                heapify(items, n, largest);
            }
        };

        auto heapSortWrapper = [&heapify](vector<StabilityTestItem>& items) {
            int n = items.size();
            
            // Build heap
            for (int i = n / 2 - 1; i >= 0; i--)
                heapify(items, n, i);
            
            // Extract elements from heap
            for (int i = n - 1; i > 0; i--) {
                // This swap can break stability
                swap(items[0], items[i]);
                heapify(items, i, 0);
            }
        };
        heapSortWrapper(testData);
    }
    else if (algorithmName == "Cocktail Sort") {
        // Cocktail sort wrapper function - stable
        auto cocktailSortWrapper = [](vector<StabilityTestItem>& items) {
            int n = items.size();
            bool swapped = true;
            int start = 0;
            int end = n - 1;
            
            while (swapped) {
                swapped = false;
                for (int i = start; i < end; i++) {
                    if (items[i].key > items[i + 1].key) {
                        // Stable swap method
                        StabilityTestItem temp = items[i];
                        items[i] = items[i + 1];
                        items[i + 1] = temp;
                        swapped = true;
                    }
                }
                
                if (!swapped) break;
                
                end--;
                swapped = false;
                
                for (int i = end - 1; i >= start; i--) {
                    if (items[i].key > items[i + 1].key) {
                        // Stable swap method
                        StabilityTestItem temp = items[i];
                        items[i] = items[i + 1];
                        items[i + 1] = temp;
                        swapped = true;
                    }
                }
                
                start++;
            }
        };
        cocktailSortWrapper(testData);
    }
    else if (algorithmName == "Library Sort") {
        // Library Sort wrapper function - stable
        auto librarySortWrapper = [](vector<StabilityTestItem>& items) {
            if (items.empty()) return;
            
            const double epsilon = 1.0;
            size_t capacity = ceil(items.size() * (1.0 + epsilon));
            vector<StabilityTestItem> gapped(capacity);
            vector<bool> isValid(capacity, false);
            
            // Insert first element
            gapped[0] = items[0];
            isValid[0] = true;
            size_t elements = 1;
            
            // Insert remaining elements
            for (size_t i = 1; i < items.size(); i++) {
                // Check if rebalancing is needed
                if (elements >= capacity / 2) {
                    // Extract elements
                    vector<StabilityTestItem> temp;
                    for (size_t j = 0; j < capacity; j++) {
                        if (isValid[j]) {
                            temp.push_back(gapped[j]);
                        }
                    }
                    
                    // Create new array
                    size_t newCapacity = ceil(elements * (1.0 + epsilon)) * 2;
                    gapped.resize(newCapacity);
                    isValid.assign(newCapacity, false);
                    
                    // Redistribute with even gaps
                    size_t gap = newCapacity / elements;
                    for (size_t j = 0; j < elements; j++) {
                        gapped[j * gap] = temp[j];
                        isValid[j * gap] = true;
                    }
                    
                    capacity = newCapacity;
                }
                
                // Find insertion position (maintain stability by inserting after equal keys)
                size_t pos = 0;
                while (pos < capacity && isValid[pos] && gapped[pos].key <= items[i].key) {
                    pos++;
                }
                
                // Make space
                size_t nextEmpty = pos;
                while (nextEmpty < capacity && isValid[nextEmpty]) {
                    nextEmpty++;
                }
                
                if (nextEmpty >= capacity) {
                    nextEmpty = capacity - 1;
                }
                
                for (size_t j = nextEmpty; j > pos; j--) {
                    gapped[j] = gapped[j-1];
                    isValid[j] = isValid[j-1];
                }
                
                // Insert element
                gapped[pos] = items[i];
                isValid[pos] = true;
                elements++;
            }
            
            // Copy results back
            size_t index = 0;
            for (size_t i = 0; i < capacity; i++) {
                if (isValid[i]) {
                    items[index++] = gapped[i];
                }
            }
        };
        librarySortWrapper(testData);
    }
    else if (algorithmName == "Tim Sort") {
        // Tim Sort wrapper function - stable
        auto timSortWrapper = [](vector<StabilityTestItem>& items) {
            const int MIN_RUN = 32;
            int n = items.size();
            
            // Calculate minimum run length
            auto calcMinRun = [](int n) {
                int r = 0;
                while (n >= MIN_RUN) {
                    r |= (n & 1);
                    n >>= 1;
                }
                return n + r;
            };
            
            // Insertion sort for small runs
            auto insertionSort = [](vector<StabilityTestItem>& items, int left, int right) {
                for (int i = left + 1; i <= right; i++) {
                    StabilityTestItem key = items[i];
                    int j = i - 1;
                    while (j >= left && items[j].key > key.key) {
                        items[j + 1] = items[j];
                        j--;
                    }
                    items[j + 1] = key;
                }
            };
            
            // Merge function
            auto merge = [](vector<StabilityTestItem>& items, int l, int m, int r) {
                int len1 = m - l + 1;
                int len2 = r - m;
                
                vector<StabilityTestItem> left(len1);
                vector<StabilityTestItem> right(len2);
                
                for (int i = 0; i < len1; i++) {
                    left[i] = items[l + i];
                }
                
                for (int i = 0; i < len2; i++) {
                    right[i] = items[m + 1 + i];
                }
                
                int i = 0, j = 0, k = l;
                
                // Stable merge
                while (i < len1 && j < len2) {
                    if (left[i].key <= right[j].key) {
                        items[k] = left[i];
                        i++;
                    } else {
                        items[k] = right[j];
                        j++;
                    }
                    k++;
                }
                
                while (i < len1) {
                    items[k] = left[i];
                    i++;
                    k++;
                }
                
                while (j < len2) {
                    items[k] = right[j];
                    j++;
                    k++;
                }
            };
            
            // Tim Sort implementation
            int minRun = calcMinRun(n);
            
            // Sort individual runs with insertion sort
            for (int i = 0; i < n; i += minRun) {
                int end = min(i + minRun - 1, n - 1);
                insertionSort(items, i, end);
            }
            
            // Merge runs
            for (int size = minRun; size < n; size = 2 * size) {
                for (int left = 0; left < n; left += 2 * size) {
                    int mid = left + size - 1;
                    int right = min(left + 2 * size - 1, n - 1);
                    
                    if (mid < right) {
                        merge(items, left, mid, right);
                    }
                }
            }
        };
        timSortWrapper(testData);
    }
    else if (algorithmName == "Comb Sort") {
        // Comb Sort wrapper function - unstable
        auto combSortWrapper = [](vector<StabilityTestItem>& items) {
            int n = items.size();
            int gap = n;
            bool swapped = true;
            
            while (gap > 1 || swapped) {
                // Update gap
                gap = (gap * 10) / 13;
                if (gap < 1) gap = 1;
                
                swapped = false;
                
                // Compare and swap with current gap
                for (int i = 0; i < n - gap; i++) {
                    if (items[i].key > items[i + gap].key) {
                        // This swap can break stability
                        swap(items[i], items[i + gap]);
                        swapped = true;
                    }
                }
            }
        };
        combSortWrapper(testData);
    }
    else if (algorithmName == "Tournament Sort") {
        // Tournament Sort wrapper function - unstable
        auto tournamentSortWrapper = [](vector<StabilityTestItem>& items) {
            int n = items.size();
            if (n <= 1) return;
            
            // Tournament tree node structure
            struct Node {
                StabilityTestItem value;
                int index;
                bool isValid;
                
                Node() : index(-1), isValid(false) {}
                Node(StabilityTestItem v, int i) : value(v), index(i), isValid(true) {}
            };
            
            // Calculate tournament tree size
            int height = static_cast<int>(ceil(log2(n)));
            int treeSize = (1 << (height + 1)) - 1;
            int leafStart = (1 << height) - 1;
            
            // Create tournament tree
            vector<Node> tree(treeSize);
            vector<StabilityTestItem> result(n);
            
            // Initialize leaf nodes
            for (int i = 0; i < n; i++) {
                tree[leafStart + i] = Node(items[i], i);
            }
            
            // Invalidate remaining leaf nodes
            for (int i = leafStart + n; i < treeSize; i++) {
                tree[i].isValid = false;
            }
            
            // Build internal nodes
            for (int i = leafStart - 1; i >= 0; i--) {
                int leftChild = 2 * i + 1;
                int rightChild = 2 * i + 2;
                
                if (!tree[leftChild].isValid) {
                    tree[i] = tree[rightChild];
                } else if (!tree[rightChild].isValid) {
                    tree[i] = tree[leftChild];
                } else if (tree[leftChild].value.key <= tree[rightChild].value.key) {
                    tree[i] = tree[leftChild];
                } else {
                    tree[i] = tree[rightChild];
                }
            }
            
            // Extract elements
            for (int i = 0; i < n; i++) {
                result[i] = tree[0].value;
                
                // Invalidate the leaf node
                int leafIndex = leafStart + tree[0].index;
                tree[leafIndex].isValid = false;
                
                // Rebuild tree
                int current = leafIndex;
                while (current > 0) {
                    int parent = (current - 1) / 2;
                    int sibling = (current % 2 == 0) ? current - 1 : current + 1;
                    
                    if (!tree[current].isValid && !tree[sibling].isValid) {
                        tree[parent].isValid = false;
                    } else if (!tree[current].isValid) {
                        tree[parent] = tree[sibling];
                    } else if (!tree[sibling].isValid) {
                        tree[parent] = tree[current];
                    } else if (tree[current].value.key <= tree[sibling].value.key) {
                        tree[parent] = tree[current];
                    } else {
                        tree[parent] = tree[sibling];
                    }
                    
                    current = parent;
                }
            }
            
            // Copy result back
            for (int i = 0; i < n; i++) {
                items[i] = result[i];
            }
        };
        tournamentSortWrapper(testData);
    }
    else if (algorithmName == "Intro Sort") {
        // Intro Sort wrapper function - unstable
        auto introSortWrapper = [](vector<StabilityTestItem>& items) {
            int n = items.size();
            
            // Insertion sort
            auto insertionSort = [](vector<StabilityTestItem>& items, int left, int right) {
                for (int i = left + 1; i <= right; i++) {
                    StabilityTestItem key = items[i];
                    int j = i - 1;
                    while (j >= left && items[j].key > key.key) {
                        items[j + 1] = items[j];
                        j--;
                    }
                    items[j + 1] = key;
                }
            };
            
            // Heap sort
            function<void(vector<StabilityTestItem>&, int, int)> heapify;
            heapify = [&heapify](vector<StabilityTestItem>& items, int n, int i) {
                int largest = i;
                int left = 2 * i + 1;
                int right = 2 * i + 2;
                
                if (left < n && items[left].key > items[largest].key)
                    largest = left;
                
                if (right < n && items[right].key > items[largest].key)
                    largest = right;
                
                if (largest != i) {
                    // This swap can break stability
                    swap(items[i], items[largest]);
                    heapify(items, n, largest);
                }
            };
            
            auto heapSort = [&heapify](vector<StabilityTestItem>& items, int left, int right) {
                int n = right - left + 1;
                vector<StabilityTestItem> subArray(items.begin() + left, items.begin() + right + 1);
                
                // Build heap
                for (int i = n / 2 - 1; i >= 0; i--)
                    heapify(subArray, n, i);
                
                // Extract elements
                for (int i = n - 1; i > 0; i--) {
                    // This swap can break stability
                    swap(subArray[0], subArray[i]);
                    heapify(subArray, i, 0);
                }
                
                // Copy back
                for (int i = 0; i < n; i++) {
                    items[left + i] = subArray[i];
                }
            };
            
            // Partition
            auto partition = [](vector<StabilityTestItem>& items, int low, int high) {
                StabilityTestItem pivot = items[high];
                int i = low - 1;
                
                for (int j = low; j <= high - 1; j++) {
                    if (items[j].key <= pivot.key) {
                        i++;
                        // This swap can break stability
                        swap(items[i], items[j]);
                    }
                }
                // This swap can break stability
                swap(items[i + 1], items[high]);
                return i + 1;
            };
            
            // Calculate max recursion depth
            int maxDepth = 2 * static_cast<int>(log2(n));
            
            // Intro Sort implementation
            function<void(vector<StabilityTestItem>&, int, int, int)> introSortUtil;
            introSortUtil = [&](vector<StabilityTestItem>& items, int left, int right, int depth) {
                int n = right - left + 1;
                
                // Use insertion sort for small arrays
                if (n <= 16) {
                    insertionSort(items, left, right);
                    return;
                }
                
                // Switch to heap sort if recursion depth exceeds limit
                if (depth == 0) {
                    heapSort(items, left, right);
                    return;
                }
                
                // Quick sort
                int pivot = partition(items, left, right);
                
                // Recursive calls
                introSortUtil(items, left, pivot - 1, depth - 1);
                introSortUtil(items, pivot + 1, right, depth - 1);
            };
            
            // Start Intro Sort
            introSortUtil(items, 0, n - 1, maxDepth);
        };
        introSortWrapper(testData);
    }
    else {
        // Unsupported algorithm
        log("Stability test not implemented for algorithm: " + algorithmName);
        return false;
    }
    
    // Verify stability: check if elements with equal keys maintain their original order
    bool isStable = true;
    
    // Group sorted data by key value
    map<int, vector<int>> keyGroups;
    for (const auto& item : testData) {
        keyGroups[item.key].push_back(item.originalPos);
    }
    
    // Check if original positions are maintained in ascending order within each group
    for (const auto& [key, positions] : keyGroups) {
        for (size_t i = 1; i < positions.size(); i++) {
            if (positions[i] < positions[i-1]) {
                isStable = false;
                log("Stability violation found: Elements with key " + to_string(key) + " do not maintain original order");
                log("Original positions: " + to_string(positions[i-1]) + " -> " + to_string(positions[i]));
                break;
            }
        }
        if (!isStable) break;
    }
    
    log("Stability test result: " + string(isStable ? "Stable" : "Unstable"));
    return isStable;
}

 
 // Run sorting algorithm (using function pointer)
 SortResult runSortingAlgorithm(const string& algorithmName, const string& datasetName, 
     vector<int>& data, 
     SortStats (*sortFunction)(vector<int>&)) {
     SortResult result;
     result.algorithmName = algorithmName;
     result.datasetName = datasetName;
     result.dataSize = data.size();
     result.isStable = ALGORITHM_STABILITY.at(algorithmName); // Theoretical stability
 
     log("Testing: " + algorithmName + " on " + datasetName);
     log("Data size: " + to_string(data.size()));
 
     // Initial memory usage (data vector size)
     size_t initialMemory = data.size() * sizeof(int);
 
     // Run test iterations
     for (int iter = 0; iter < TEST_ITERATIONS; iter++) {
         // Copy original data (maintain same initial state for each iteration)
         vector<int> testData = data;
 
         log("Iteration " + to_string(iter + 1) + "/" + to_string(TEST_ITERATIONS) + " started...");
 
         // Measure sorting time
         auto start = high_resolution_clock::now();
         SortStats stats = sortFunction(testData);
         auto end = high_resolution_clock::now();
 
         auto duration = duration_cast<milliseconds>(end - start);
         result.executionTimes.push_back(duration.count());
 
         // Only verify sorting and store statistics on first iteration
         if (iter == 0) {
             result.isSorted = verifySorting(testData);
             result.comparisons = stats.comparisons;
             result.assignments = stats.assignments;
             result.memoryUsage = initialMemory + stats.memoryUsage;
 
             log("Sorting verification: " + string(result.isSorted ? "Success" : "Failed"));
             log("Comparison operations: " + to_string(stats.comparisons));
             log("Assignment operations: " + to_string(stats.assignments));
             log("Additional memory usage: " + to_string(stats.memoryUsage) + " bytes");
             log("Total memory usage: " + to_string(result.memoryUsage) + " bytes");
         }
 
         log("Execution time: " + to_string(duration.count()) + " milliseconds");
     }
 
     // Calculate average execution time
     result.avgExecutionTime = accumulate(result.executionTimes.begin(), result.executionTimes.end(), 0.0) / 
        result.executionTimes.size();
 
     log("Average execution time: " + to_string(result.avgExecutionTime) + " milliseconds");
 
     // Perform stability test (with smaller data size)
     const int STABILITY_TEST_SIZE = 1000; // Stability test data size
     result.measuredStability = testStability(algorithmName, STABILITY_TEST_SIZE);
 
     log("Theoretical stability: " + string(result.isStable ? "Stable" : "Unstable"));
     log("Measured stability: " + string(result.measuredStability ? "Stable" : "Unstable"));
 
     // Check if theoretical and measured stability match
     if (result.isStable != result.measuredStability) {
         log("Warning: Theoretical stability and measured stability do not match!");
     }
 
     log("------------------------");
 
     return result;
 }
 

 // Test all sorting algorithms
vector<SortResult> testAllSortingAlgorithms(const string& dataFile) {
    vector<int> data;
    log("Loading file: " + dataFile);
    if (!DataGenerator::loadFromFile(dataFile, data)) {
        log("Failed to load file: " + dataFile);
        return {};
    }

    string datasetName = fs::path(dataFile).filename().string();
    vector<SortResult> results;

    // Test all sorting algorithms
    results.push_back(runSortingAlgorithm("Bubble Sort", datasetName, data, BubbleSort::sort));
    results.push_back(runSortingAlgorithm("Insertion Sort", datasetName, data, InsertionSort::sort));
    results.push_back(runSortingAlgorithm("Selection Sort", datasetName, data, SelectionSort::sort));
    results.push_back(runSortingAlgorithm("Quick Sort", datasetName, data, QuickSort::sort));
    results.push_back(runSortingAlgorithm("Merge Sort", datasetName, data, MergeSort::sort));
    results.push_back(runSortingAlgorithm("Heap Sort", datasetName, data, HeapSort::sort));

    results.push_back(runSortingAlgorithm("Library Sort", datasetName, data, LibrarySort::sort));
    results.push_back(runSortingAlgorithm("Tim Sort", datasetName, data, TimSort::sort));
    results.push_back(runSortingAlgorithm("Cocktail Sort", datasetName, data, CocktailSort::sort));
    results.push_back(runSortingAlgorithm("Comb Sort", datasetName, data, CombSort::sort));
    results.push_back(runSortingAlgorithm("Tournament Sort", datasetName, data, TournamentSort::sort));
    results.push_back(runSortingAlgorithm("Intro Sort", datasetName, data, IntroSort::sort));

    return results;
}

// Print results in table format
void printResultsTable(const vector<SortResult>& results) {
    // Print header
    cout << setw(20) << left << "Algorithm" 
         << setw(15) << left << "Dataset" 
         << setw(10) << right << "Size" 
         << setw(15) << right << "Avg Time(ms)" 
         << setw(15) << right << "Comparisons" 
         << setw(15) << right << "Assignments" 
         << setw(15) << right << "Memory(bytes)" 
         << setw(12) << left << "Theoretical" 
         << setw(12) << left << "Measured" 
         << setw(10) << left << "Sorted" 
         << endl;

    cout << string(140, '-') << endl;

    // Print results
    for (const auto& result : results) {
        cout << setw(20) << left << result.algorithmName 
             << setw(15) << left << result.datasetName.substr(0, 14)
             << setw(10) << right << result.dataSize 
             << setw(15) << right << fixed << setprecision(2) << result.avgExecutionTime 
             << setw(15) << right << result.comparisons 
             << setw(15) << right << result.assignments 
             << setw(15) << right << result.memoryUsage 
             << setw(12) << left << (result.isStable ? "Stable" : "Unstable") 
             << setw(12) << left << (result.measuredStability ? "Stable" : "Unstable") 
             << setw(10) << left << (result.isSorted ? "Success" : "Failed") 
             << endl;
    }
}

// Compare algorithm performance
void compareAlgorithmPerformance(const vector<SortResult>& allResults) {
    map<string, map<string, double>> algorithmPerformance;

    // Calculate average execution time by algorithm and dataset type
    for (const auto& result : allResults) {
        string dataType;
        if (result.datasetName.find("sorted_asc") != string::npos) {
            dataType = "Sorted(Asc)";
        } else if (result.datasetName.find("sorted_desc") != string::npos) {
            dataType = "Sorted(Desc)";
        } else if (result.datasetName.find("random") != string::npos) {
            dataType = "Random";
        } else if (result.datasetName.find("partially_sorted") != string::npos) {
            dataType = "Partially Sorted";
        } else if (result.datasetName.find("reverse_sorted") != string::npos) {
            dataType = "Reverse Sorted";
        } else {
            dataType = "Other";
        }

        algorithmPerformance[result.algorithmName][dataType] = result.avgExecutionTime;
    }

    // Print results
    cout << "\n===== Algorithm Performance Comparison =====\n" << endl;

    cout << setw(20) << left << "Algorithm" 
         << setw(20) << right << "Sorted(Asc)" 
         << setw(20) << right << "Sorted(Desc)" 
         << setw(20) << right << "Random" 
         << setw(20) << right << "Partially Sorted" 
         << setw(20) << right << "Reverse Sorted" 
         << endl;

    cout << string(120, '-') << endl;

    for (const auto& [algorithm, performance] : algorithmPerformance) {
        cout << setw(20) << left << algorithm;

        for (const auto& dataType : {"Sorted(Asc)", "Sorted(Desc)", "Random", "Partially Sorted", "Reverse Sorted"}) {
            if (performance.find(dataType) != performance.end()) {
                cout << setw(20) << right << fixed << setprecision(2) << performance.at(dataType);
            } else {
                cout << setw(20) << right << "N/A";
            }
        }

        cout << endl;
    }

    // Print algorithm theoretical complexity and stability
    cout << "\n===== Algorithm Theoretical Complexity and Stability =====\n" << endl;

    cout << setw(20) << left << "Algorithm" 
         << setw(15) << right << "Best" 
         << setw(15) << right << "Average" 
         << setw(15) << right << "Worst" 
         << setw(15) << right << "Theoretical" 
         << setw(15) << right << "Measured" 
         << endl;

    cout << string(95, '-') << endl;

    // Collect measured stability information for each algorithm
    map<string, bool> measuredStability;
    for (const auto& result : allResults) {
        measuredStability[result.algorithmName] = result.measuredStability;
    }

    for (const auto& [algorithm, complexity] : ALGORITHM_COMPLEXITY) {
        bool theoreticalStability = ALGORITHM_STABILITY.at(algorithm);
        bool actualStability = measuredStability.count(algorithm) ? measuredStability[algorithm] : false;

        cout << setw(20) << left << algorithm
             << setw(15) << right << complexity.at("Best")
             << setw(15) << right << complexity.at("Average")
             << setw(15) << right << complexity.at("Worst")
             << setw(15) << right << (theoreticalStability ? "Stable" : "Unstable")
             << setw(15) << right << (actualStability ? "Stable" : "Unstable");

        // Mark if theory and practice differ
        if (theoreticalStability != actualStability && measuredStability.count(algorithm)) {
            cout << " ⚠️";  // Warning indicator
        }

        cout << endl;
    }
}

// Export summary data to CSV file by algorithm, dataset type, and size
void exportSummaryToCSV(const vector<SortResult>& results, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        log("Cannot open CSV file: " + filename);
        return;
    }

    // Extract and organize dataset types
    map<string, string> datasetTypeMap;
    for (const auto& result : results) {
        string dataType;
        if (result.datasetName.find("sorted_asc") != string::npos) {
            dataType = "Sorted(Asc)";
        } else if (result.datasetName.find("sorted_desc") != string::npos) {
            dataType = "Sorted(Desc)";
        } else if (result.datasetName.find("random") != string::npos) {
            dataType = "Random";
        } else if (result.datasetName.find("partially_sorted") != string::npos) {
            dataType = "Partially Sorted";
        } else if (result.datasetName.find("reverse_sorted") != string::npos) {
            dataType = "Reverse Sorted";
        } else {
            dataType = "Other";
        }
        datasetTypeMap[result.datasetName] = dataType;
    }

    // Collect performance data by algorithm, dataset type, and size
    map<tuple<string, string, int>, SortResult> summaryData;
    for (const auto& result : results) {
        string dataType = datasetTypeMap[result.datasetName];
        auto key = make_tuple(result.algorithmName, dataType, result.dataSize);
        summaryData[key] = result;
    }
    
    // Write CSV header
    file << "Algorithm,Dataset Type,Data Size,Average Execution Time(ms),Comparisons,Assignments,Memory Usage(bytes),Theoretical Stability,Measured Stability\n";
    
    // Write data
    for (const auto& [key, result] : summaryData) {
        const auto& [algorithm, dataType, size] = key;
        file << algorithm << ","
             << dataType << ","
             << size << ","
             << fixed << setprecision(2) << result.avgExecutionTime << ","
             << result.comparisons << ","
             << result.assignments << ","
             << result.memoryUsage << ","
             << (result.isStable ? "Stable" : "Unstable") << ","
             << (result.measuredStability ? "Stable" : "Unstable") << "\n";
    }
    
    file.close();
    log("Summary data saved to CSV file: " + filename);
}

// Summarize stability test results
void summarizeStabilityTests(const vector<SortResult>& results) {
    cout << "\n===== Stability Test Results Summary =====\n" << endl;
    
    cout << setw(20) << left << "Algorithm" 
         << setw(15) << right << "Theoretical" 
         << setw(15) << right << "Measured" 
         << setw(20) << right << "Match" 
         << endl;
    
    cout << string(70, '-') << endl;
    
    // Remove duplicates by algorithm
    map<string, pair<bool, bool>> stabilityResults;
    for (const auto& result : results) {
        stabilityResults[result.algorithmName] = {result.isStable, result.measuredStability};
    }
    
    for (const auto& [algorithm, stability] : stabilityResults) {
        bool theoretical = stability.first;
        bool measured = stability.second;
        bool match = theoretical == measured;
        
        cout << setw(20) << left << algorithm
             << setw(15) << right << (theoretical ? "Stable" : "Unstable")
             << setw(15) << right << (measured ? "Stable" : "Unstable")
             << setw(20) << right << (match ? "Match" : "Mismatch ⚠️")
             << endl;
    }
    
    // Analyze stability test results
    int matchCount = 0;
    for (const auto& [algorithm, stability] : stabilityResults) {
        if (stability.first == stability.second) {
            matchCount++;
        }
    }
    
    double matchPercentage = (double)matchCount / stabilityResults.size() * 100;
    cout << "\nMatch rate between theoretical and measured stability: " 
         << fixed << setprecision(2) << matchPercentage << "%" << endl;
    
    if (matchPercentage < 100) {
        cout << "\nWarning: Some algorithms' theoretical stability does not match measured stability." << endl;
        cout << "This may be due to implementation differences or limitations in the testing method." << endl;
    }
}

int main() {
    // Set data sizes (1K, 10K, 100K, 1M)
    vector<int> sizes = {1000, 10000, 100000, 1000000};
    
    // Create data directory
    string dataDir = "data";
    if (!fs::exists(dataDir)) {
        log("Creating data directory: " + dataDir);
        fs::create_directories(dataDir);
        
        log("Generating test data...");
        
        // Generate and save datasets for each size
        for (int size : sizes) {
            log("Generating datasets of size " + to_string(size) + "...");
            
            // Ascending sorted data
            log("  - Generating ascending sorted data");
            vector<int> sortedAsc = DataGenerator::generateSortedData(size, true);
            DataGenerator::saveToFile(dataDir + "/sorted_asc_" + to_string(size) + ".txt", sortedAsc);
            
            // Descending sorted data
            log("  - Generating descending sorted data");
            vector<int> sortedDesc = DataGenerator::generateSortedData(size, false);
            DataGenerator::saveToFile(dataDir + "/sorted_desc_" + to_string(size) + ".txt", sortedDesc);
            
            // Random data
            log("  - Generating random data");
            vector<int> random = DataGenerator::generateRandomData(size);
            DataGenerator::saveToFile(dataDir + "/random_" + to_string(size) + ".txt", random);
            
            // Partially sorted data
            log("  - Generating partially sorted data");
            vector<int> partiallySorted = DataGenerator::generatePartiallySortedData(size);
            DataGenerator::saveToFile(dataDir + "/partially_sorted_" + to_string(size) + ".txt", partiallySorted);
            
            // Reverse sorted data
            log("  - Generating reverse sorted data");
            vector<int> reverseSorted = DataGenerator::generateReverseSortedData(size);
            DataGenerator::saveToFile(dataDir + "/reverse_sorted_" + to_string(size) + ".txt", reverseSorted);
            
            log("Completed generating datasets of size " + to_string(size));
        }
        
        log("All test data has been generated.");
    } else {
        log("Using existing data directory: " + dataDir);
    }
    
    // Provide options for data size selection
    log("\n===== Starting Sorting Algorithm Performance Evaluation =====\n");
    log("Select data size to test:");
    log("1: Small size (1K)");
    log("2: Medium size (1K, 10K)");
    log("3: Large size (1K, 10K, 100K)");
    log("4: All sizes (1K, 10K, 100K, 1M) - Warning: This may take a very long time");
    log("5: Run stability tests only (1K)");
    
    int choice;
    cout << "Select (1-5): ";
    cin >> choice;
    
    vector<int> testSizes;
    bool stabilityTestOnly = false;
    
    switch (choice) {
        case 1:
            testSizes = {1000};
            break;
        case 2:
            testSizes = {1000, 10000};
            break;
        case 3:
            testSizes = {1000, 10000, 100000};
            break;
        case 4:
            testSizes = {1000, 10000, 100000, 1000000};
            break;
        case 5:
            testSizes = {1000};
            stabilityTestOnly = true;
            break;
        default:
            log("Invalid selection. Testing small size only.");
            testSizes = {1000};
    }
    
    // Vector to store all results
    vector<SortResult> allResults;
    
    if (stabilityTestOnly) {
        log("\n===== Running Stability Tests Only =====\n");
        
        // List of algorithms for stability testing
        vector<string> algorithms = {
            "Bubble Sort", "Insertion Sort", "Selection Sort", 
            "Quick Sort", "Merge Sort", "Heap Sort",
            "Library Sort", "Tim Sort", "Cocktail Sort",
            "Comb Sort", "Tournament Sort", "Intro Sort"
        };
        
        for (const auto& algorithm : algorithms) {
            bool measuredStability = testStability(algorithm, 1000);
            log(algorithm + " stability test result: " + string(measuredStability ? "Stable" : "Unstable"));
            log("Theoretical stability: " + string(ALGORITHM_STABILITY.at(algorithm) ? "Stable" : "Unstable"));
            
            // Store results
            SortResult result;
            result.algorithmName = algorithm;
            result.isStable = ALGORITHM_STABILITY.at(algorithm);
            result.measuredStability = measuredStability;
            allResults.push_back(result);
        }
        
        // Summarize stability test results
        summarizeStabilityTests(allResults);
    } else {
        // Test selected data sizes
        for (int size : testSizes) {
            log("\nStarting tests for size " + to_string(size) + "\n");
            
            // Test all sorting algorithms on each dataset type
            vector<string> dataTypes = {
                "/sorted_asc_", 
                "/sorted_desc_", 
                "/random_", 
                "/partially_sorted_", 
                "/reverse_sorted_"
            };
            
            for (const auto& dataType : dataTypes) {
                string dataFile = dataDir + dataType + to_string(size) + ".txt";
                vector<SortResult> results = testAllSortingAlgorithms(dataFile);
                allResults.insert(allResults.end(), results.begin(), results.end());
            }
            
            log("Completed tests for size " + to_string(size));
        }
        
        // Print results
        log("\n===== Sorting Algorithm Performance Evaluation Results =====\n");
        printResultsTable(allResults);
        
        // Compare algorithm performance
        compareAlgorithmPerformance(allResults);
        
        // Summarize stability test results
        summarizeStabilityTests(allResults);
        
        // Export summary data to CSV file
        string csvFilename = "sorting_summary_" + to_string(testSizes.back()) + ".csv";
        exportSummaryToCSV(allResults, csvFilename);
        
        log("\n===== Sorting Algorithm Performance Evaluation Complete =====\n");
        log("Summary data has been saved to " + csvFilename);
    }
    
    return 0;
}
