#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include "data-structures/Graph.h"
#include "data-structures/unorderedSet.h"

// Rest of the file remains the same
class SudokuBoard {
private:
    Graph constraintGraph;
    int board[9][9]; // For easy access to the current state
    
    // Initialize the constraint graph with all vertices and edges
    void initializeGraph();
    
    // Add all constraints (edges) to the graph
    void addConstraints();
    
    // Update the internal board array from the graph
    void updateBoardFromGraph();
    
public:
    SudokuBoard();
    
    // Insert a value into the board
    bool insert(int row, int col, int value);
    
    // Propagate constraints after setting a value
    void propagateConstraints(int row, int col, int value);
    unorderedSet calculateDomain(int row, int col) const;
    
    // Remove a value from the board (reset cell)
    bool remove(int row, int col);
    
    // Recalculate all domains based on the current board state
    void recalculateDomains();
    
    // Get the value at a specific position
    int getValue(int row, int col) const;
    
    // Get the domain of a cell
    unorderedSet getDomain(int row, int col) const;
    
    // Print the current state of the board
    void printBoard() const;
    
    // Load a board from a 2D array
    void loadBoard(const int inputBoard[9][9]);
    
    // Get the graph for the solver to use
    const Graph& getGraph() const;
    
    // Get a modifiable reference to the graph (for the solver)
    Graph& getGraphRef();
    
    // Get the current board state as a 2D array
    void getBoardState(int outputBoard[9][9]) const;
    
    // Check if a position is empty
    bool isEmpty(int row, int col) const;
    
    // Clear the entire board
    void clear();
};

#endif // SUDOKU_BOARD_H
