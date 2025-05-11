#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include "data-structures/graph.h"
#include "data-structures/unorderedSet.h"

class SudokuBoard {
private:
    Graph constraintGraph;

    bool fixedCells[9][9]; // To mark fixed (initial) cells

    // Initialize the constraint graph with all vertices and edges
    void initializeGraph();

    // Add all constraints (edges) to the graph
    void addConstraints();

    // Update the internal board array from the graph
    void updateBoardFromGraph();

public:
    SudokuBoard();
    int board[9][9]; // For easy access to the current state
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

    // Check if a cell is fixed (part of the original puzzle)
    bool isFixedCell(int row, int col) const;
    
    // Clear the entire board
    void clear();
};

#endif // SUDOKUBOARD_H
