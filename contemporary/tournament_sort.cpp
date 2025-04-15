#include "tournament_sort.h"
#include <cmath>
#include <limits>
#include <algorithm>

/**************************************************************************************
 * TournamentSort::getName
 * 
 * Returns the name of the sorting algorithm.
 * 
 * Output: String containing the name "Tournament Sort"
 **************************************************************************************/
std::string TournamentSort::getName() {
    return "Tournament Sort";
}

/**************************************************************************************
 * TournamentSort::sort
 * 
 * Main function to execute the Tournament Sort algorithm on an array.
 * 
 * The algorithm works by:
 * 1. Building a tournament tree where each node is the minimum of its children
 * 2. Extracting the minimum element (root) and placing it in the result array
 * 3. Invalidating the corresponding leaf node and rebuilding the tree
 * 4. Repeating until all elements are extracted
 * 
 * Input: Array to be sorted
 * Output: Statistics about comparisons and assignments during sorting
 **************************************************************************************/
SortStats TournamentSort::sort(std::vector<int>& arr) {
    SortStats stats;
    
    int n = arr.size();
    if (n <= 1) {
        return stats;
    }
    
    // Calculate tournament tree size (complete binary tree)
    int height = static_cast<int>(std::ceil(std::log2(n)));
    int treeSize = (1 << (height + 1)) - 1;
    int leafStart = (1 << height) - 1; // Starting index of leaf nodes
    
    // Create tournament tree
    std::vector<Node> tree(treeSize);
    stats.memoryUsage += treeSize * sizeof(Node);
    
    // Create result array
    std::vector<int> result(n);
    stats.memoryUsage += n * sizeof(int);
    
    // Build initial tournament tree
    buildTournamentTree(arr, tree, stats);
    
    // Extract all elements
    for (int i = 0; i < n; i++) {
        // Root node contains the current minimum
        result[i] = tree[0].value;
        stats.assignments++;
        
        // Invalidate the corresponding leaf node and mark with max value
        int leafIndex = leafStart + tree[0].index;
        tree[leafIndex].isValid = false;
        tree[leafIndex].value = std::numeric_limits<int>::max();
        stats.assignments += 2;
        
        // Rebuild the tree
        rebuildTree(tree, leafIndex, stats);
    }
    
    // Copy sorted result back to original array
    for (int i = 0; i < n; i++) {
        arr[i] = result[i];
        stats.assignments++;
    }
    
    return stats;
}

/**************************************************************************************
 * TournamentSort::buildTournamentTree
 * 
 * Builds the initial tournament tree from the input array.
 * 
 * Inputs:
 * - Original array
 * - Tournament tree to be built
 * - Statistics object to track operations
 **************************************************************************************/
void TournamentSort::buildTournamentTree(const std::vector<int>& arr, std::vector<Node>& tree, SortStats& stats) {
    int n = arr.size();
    int treeSize = tree.size();
    int height = static_cast<int>(std::log2(treeSize + 1)) - 1;
    int leafStart = (1 << height) - 1; // Starting index of leaf nodes
    
    // Initialize leaf nodes with array values
    for (int i = 0; i < n; i++) {
        tree[leafStart + i] = Node(arr[i], i);
        stats.assignments++;
    }
    
    // Initialize remaining leaf nodes as invalid with max value
    for (int i = leafStart + n; i < treeSize; i++) {
        tree[i].isValid = false;
        tree[i].value = std::numeric_limits<int>::max();
        stats.assignments += 2;
    }
    
    // Build internal nodes (bottom-up)
    for (int i = leafStart - 1; i >= 0; i--) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        
        // Select smaller of the two children
        stats.comparisons++;
        if (tree[leftChild].value <= tree[rightChild].value) {
            tree[i] = tree[leftChild];
        } else {
            tree[i] = tree[rightChild];
        }
        stats.assignments++;
    }
}

/**************************************************************************************
 * TournamentSort::rebuildTree
 * 
 * Rebuilds the tournament tree from a specific node upward.
 * Called after a leaf node has been invalidated.
 * 
 * Inputs:
 * - Tournament tree
 * - Index of the node to start rebuilding from
 * - Statistics object to track operations
 **************************************************************************************/
void TournamentSort::rebuildTree(std::vector<Node>& tree, int nodeIndex, SortStats& stats) {
    if (nodeIndex == 0) return; // If root node, nothing to rebuild
    
    // Calculate parent node
    int parent = (nodeIndex - 1) / 2;
    
    // Calculate sibling node
    int sibling = (nodeIndex % 2 == 0) ? nodeIndex - 1 : nodeIndex + 1;
    
    // Update parent node with the smaller of the two children
    stats.comparisons++;
    if (tree[nodeIndex].value <= tree[sibling].value) {
        tree[parent] = tree[nodeIndex];
    } else {
        tree[parent] = tree[sibling];
    }
    stats.assignments++;
    
    // Recursively rebuild up the tree
    rebuildTree(tree, parent, stats);
}
