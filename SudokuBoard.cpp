#include "SudokuBoard.h"
#include <iostream>

// Constructor
SudokuBoard::SudokuBoard() {
    // Initialize the board with zeros
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            board[row][col] = 0;
        }
    }
    
    // Initialize the constraint graph
    initializeGraph();
}

// Initialize the constraint graph with all vertices and edges
void SudokuBoard::initializeGraph() {
    // Add all cells as vertices
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            constraintGraph.addVertex(row, col);
        }
    }
    
    // Add all constraints as edges
    addConstraints();
}

// Add all constraints (edges) to the graph
void SudokuBoard::addConstraints() {
    // Row constraints
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            for (int otherCol = 0; otherCol < 9; otherCol++) {
                if (col != otherCol) {
                    constraintGraph.addEdge(row, col, row, otherCol);
                }
            }
        }
    }
    
    // Column constraints
    for (int col = 0; col < 9; col++) {
        for (int row = 0; row < 9; row++) {
            for (int otherRow = 0; otherRow < 9; otherRow++) {
                if (row != otherRow) {
                    constraintGraph.addEdge(row, col, otherRow, col);
                }
            }
        }
    }
    
    // 3x3 box constraints
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            for (int cellRow = 0; cellRow < 3; cellRow++) {
                for (int cellCol = 0; cellCol < 3; cellCol++) {
                    int row = boxRow * 3 + cellRow;
                    int col = boxCol * 3 + cellCol;
                    
                    // Connect to all other cells in the same 3x3 box
                    for (int otherCellRow = 0; otherCellRow < 3; otherCellRow++) {
                        for (int otherCellCol = 0; otherCellCol < 3; otherCellCol++) {
                            int otherRow = boxRow * 3 + otherCellRow;
                            int otherCol = boxCol * 3 + otherCellCol;
                            
                            if (row != otherRow || col != otherCol) {
                                constraintGraph.addEdge(row, col, otherRow, otherCol);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Update the internal board array from the graph
void SudokuBoard::updateBoardFromGraph() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (constraintGraph.isSingleValue(row, col)) {
                board[row][col] = constraintGraph.getValue(row, col);
            } else {
                board[row][col] = 0; // Unsolved cells are 0
            }
        }
    }
}

// Insert a value into the board
bool SudokuBoard::insert(int row, int col, int value) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9 || value < 1 || value > 9) {
        return false; // Invalid input
    }
    
    // Set the value in the constraint graph
    bool result = constraintGraph.setValue(row, col, value);
    
    if (result) {
        // Propagate constraints to neighbors
        propagateConstraints(row, col, value);
        
        // Update the board array
        board[row][col] = value;
    }
    
    return result;
}

// Propagate constraints after setting a value
void SudokuBoard::propagateConstraints(int row, int col, int value) {
    // Get all neighbors of this cell
    CustomVector<CustomPair<int, int>> neighbors = constraintGraph.getNeighbors(row, col);
    
    // Remove the value from the domains of all neighbors
    for (int i = 0; i < neighbors.size(); i++) {
        constraintGraph.removeFromDomain(neighbors[i].first, neighbors[i].second, value);
    }
}

// Remove a value from the board (reset cell)
bool SudokuBoard::remove(int row, int col) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) {
        return false; // Invalid input
    }
    
    // Reset the domain to all possible values
    unorderedSet fullDomain;
    for (int i = 1; i <= 9; i++) {
        fullDomain.insert(i);
    }
    
    bool result = constraintGraph.setDomain(row, col, fullDomain);
    
    if (result) {
        // Update the board array
        board[row][col] = 0;
        
        // We need to recalculate the domains based on the current board state
        recalculateDomains();
    }
    
    return result;
}

// Recalculate all domains based on the current board state
void SudokuBoard::recalculateDomains() {
    // First, reset all domains for empty cells
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                unorderedSet fullDomain;
                for (int i = 1; i <= 9; i++) {
                    fullDomain.insert(i);
                }
                constraintGraph.setDomain(row, col, fullDomain);
            }
        }
    }
    
    // Then, propagate constraints from filled cells
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] != 0) {
                propagateConstraints(row, col, board[row][col]);
            }
        }
    }
}

// Get the value at a specific position
int SudokuBoard::getValue(int row, int col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) {
        return -1; // Invalid input
    }
    return board[row][col];
}

unorderedSet SudokuBoard::calculateDomain(int row, int col) const {
    unorderedSet domain;
    
    // Start with full domain {1..9}
    for (int i = 1; i <= 9; i++) {
        domain.insert(i);
    }

    // Remove values from the same row
    for (int c = 0; c < 9; c++) {
        if (constraintGraph.isSingleValue(row, c) && c != col) {
            int val = constraintGraph.getValue(row, c);
            domain.remove(val);
        }
    }

    // Remove values from the same column
    for (int r = 0; r < 9; r++) {
        if (constraintGraph.isSingleValue(r, col) && r != row) {
            int val = constraintGraph.getValue(r, col);
            domain.remove(val);
        }
    }

    // Remove values from the same 3x3 box
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (constraintGraph.isSingleValue(r, c) && (r != row || c != col)) {
                int val = constraintGraph.getValue(r, c);
                domain.remove(val);
            }
        }
    }

    return domain;
}

// Print the current state of the board
void SudokuBoard::printBoard() const {
    std::cout << "-------------------------" << std::endl;
    for (int row = 0; row < 9; row++) {
        std::cout << "| ";
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                std::cout << ". ";
            } else {
                std::cout << board[row][col] << " ";
            }
            if ((col + 1) % 3 == 0) {
                std::cout << "| ";
            }
        }
        std::cout << std::endl;
        if ((row + 1) % 3 == 0) {
            std::cout << "-------------------------" << std::endl;
        }
    }
}

// Load a board from a 2D array
void SudokuBoard::loadBoard(const int inputBoard[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (inputBoard[row][col] != 0) {
                insert(row, col, inputBoard[row][col]);
            } else {
                // Make sure empty cells are properly reset
                remove(row, col);
            }
        }
    }
}

// Get the graph for the solver to use
const Graph& SudokuBoard::getGraph() const {
    return constraintGraph;
}

// Get a modifiable reference to the graph (for the solver)
Graph& SudokuBoard::getGraphRef() {
    return constraintGraph;
}

// Get the current board state as a 2D array
void SudokuBoard::getBoardState(int outputBoard[9][9]) const {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            outputBoard[row][col] = board[row][col];
        }
    }
}

// Check if a position is empty
bool SudokuBoard::isEmpty(int row, int col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) {
        return false; // Invalid position
    }
    return board[row][col] == 0;
}

// Clear the entire board
void SudokuBoard::clear() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            remove(row, col);
        }
    }
}
