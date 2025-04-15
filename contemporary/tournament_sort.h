#ifndef TOURNAMENT_SORT_H
#define TOURNAMENT_SORT_H

#include "sorting_algorithms.h"
#include <vector>
#include <string>

/**************************************************************************************
 * Tournament Sort Implementation
 * 
 * A sorting algorithm that uses a tournament tree (or winner tree) to efficiently
 * find the minimum element in each iteration.
 * 
 * Key characteristics:
 * - Uses a complete binary tree to organize comparisons
 * - Each node represents the winner (smaller element) of its children
 * - Root node always contains the current minimum element
 * - After extracting the minimum, the tree is rebuilt
 * - Time complexity: O(n log n)
 * - Space complexity: O(n) for the tournament tree
 **************************************************************************************/
class TournamentSort {
public:
    /**
     * Execute Tournament Sort algorithm
     * 
     * Takes an array to be sorted and returns sorting statistics.
     */
    static SortStats sort(std::vector<int>& arr);
    
    /**
     * Return algorithm name
     * 
     * Returns the name of this sorting algorithm.
     */
    static std::string getName();
    
private:
    /**
     * Tournament tree node structure
     * 
     * Represents a node in the tournament tree with its value,
     * original index, and validity status.
     */
    struct Node {
        int value;      // Node value
        int index;      // Original index in the array
        bool isValid;   // Whether the node is valid
        
        Node() : value(0), index(-1), isValid(false) {}
        Node(int v, int i) : value(v), index(i), isValid(true) {}
    };
    
    /**
     * Build tournament tree
     * 
     * Constructs the initial tournament tree from the input array.
     */
    static void buildTournamentTree(const std::vector<int>& arr, std::vector<Node>& tree, SortStats& stats);
    
    /**
     * Extract minimum and rebuild tree
     * 
     * Extracts the minimum element from the tree and rebuilds it.
     */
    static Node extractMinAndRebuild(const std::vector<int>& arr, std::vector<Node>& tree, SortStats& stats);
    
    /**
     * Rebuild tree
     * 
     * Rebuilds the tournament tree from a specific node upward after
     * a leaf node has been modified.
     */
    static void rebuildTree(std::vector<Node>& tree, int nodeIndex, SortStats& stats);
};

#endif // TOURNAMENT_SORT_H
