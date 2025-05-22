#ifndef GRAPH_H
#define GRAPH_H

#include "unorderedSet.h"
#include "CustomVector.h"
#include "CustomPair.h"

class Graph {
private:
    // Each cell in the Sudoku is a vertex in our graph
    struct Vertex {
        int row, col;  // Position in the Sudoku board
        unorderedSet domain;  // Possible values for this cell
        CustomVector<CustomPair<int, int>> neighbors;  // Adjacent vertices
        
        Vertex(int r, int c);
    };
    
    // Custom map implementation for vertices
    static const int MAX_VERTICES = 81; // 9x9 board
    Vertex* vertices[MAX_VERTICES];
    bool vertexExists[MAX_VERTICES];
    int numVertices;
    
    // Convert row, col to a unique vertex ID
    int getVertexId(int row, int col) const;
    
public:
    Graph();
    ~Graph();
    
    // Add a vertex to the graph
    void addVertex(int row, int col);
    
    // Add an edge between two vertices
    void addEdge(int row1, int col1, int row2, int col2);
    void debugPrint() const; // For debugging purposes
    void connectSudokuConstraints(); // Connect all constraints in Sudoku
    
    // Get neighbors of a vertex
    CustomVector<CustomPair<int, int>> getNeighbors(int row, int col) const;
    
    // Set the domain of a vertex (possible values for a cell)
    bool setDomain(int row, int col, const unorderedSet& domain);
    
    // Set a single value in the domain (for initial board setup)
    bool setValue(int row, int col, int value);
    
    // Get the domain of a vertex
    unorderedSet getDomain(int row, int col) const;
    
    // Remove a value from the domain of a vertex
    bool removeFromDomain(int row, int col, int value);
    
    // Check if a vertex has a single value in its domain
    bool isSingleValue(int row, int col) const;
    
    // Get the single value in the domain if it has only one value
    int getValue(int row, int col) const;
    
    // Check if the domain is empty (no valid values)
    bool isDomainEmpty(int row, int col) const;
};

#endif // GRAPH_H
