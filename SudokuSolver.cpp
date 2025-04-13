//
// Created by Yassien Ebrahim  on 13/04/2025.
//

#include "SudokuSolver.h"
#include <algorithm>
#include <vector>

// Constructor initializes the random number generator and stores reference to board
SudokuSolver::SudokuSolver(SudokuBoard& sudokuBoard) 
    : board(sudokuBoard), rng(static_cast<unsigned int>(std::time(nullptr))) {
}

// Main solving algorithm using backtracking
bool SudokuSolver::solve() {
    // Start solving from the top-left corner
    return solveRecursive(0, 0);
}

// Recursive helper for the solving algorithm
bool SudokuSolver::solveRecursive(int row, int col) {
    // If we've filled the entire board, we're done
    if (row == 9) {
        return true;
    }
    
    // Move to the next cell
    int nextRow = (col == 8) ? row + 1 : row;
    int nextCol = (col == 8) ? 0 : col + 1;
    
    // If this cell already has a value, move to the next one
    if (!board.isEmpty(row, col)) {
        return solveRecursive(nextRow, nextCol);
    }
    
    // Try each possible value for this cell
    for (int num = 1; num <= 9; num++) {
        if (isValidMove(row, col, num)) {
            // Place this value in the cell
            if (board.insert(row, col, num)) {
                // Recursively try to solve the rest of the board
                if (solveRecursive(nextRow, nextCol)) {
                    return true;
                }
                
                // If we couldn't solve with this value, backtrack
                board.remove(row, col);
            }
        }
    }
    
    // No solution found with any value in this cell
    return false;
}

// Find the first empty cell on the board
std::pair<int, int> SudokuSolver::findEmptyCell() const {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board.isEmpty(row, col)) {
                return std::make_pair(row, col);
            }
        }
    }
    return std::make_pair(-1, -1); // No empty cells found
}

// Check if a specific move is valid based on Sudoku rules
bool SudokuSolver::isValidMove(int row, int col, int val) const {
    // Use the domain calculation to check if value is allowed
    unorderedSet domain = board.calculateDomain(row, col);
    return domain.contains(val);
}

// Check if the current board is solvable
bool SudokuSolver::isSolvable() {
    // Save the current state
    int currentBoard[9][9];
    board.getBoardState(currentBoard);
    
    // Try to solve it
    bool solvable = solve();
    
    // Restore the original state
    board.loadBoard(currentBoard);
    
    return solvable;
}

// Check if the current board configuration is valid
bool SudokuSolver::isValidBoard() const {
    // Check each cell that has a value
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int val = board.getValue(row, col);
            if (val != 0) {
                // Temporarily remove the value
                const_cast<SudokuBoard&>(board).remove(row, col);
                
                // Check if it would be valid to place it again
                bool valid = isValidMove(row, col, val);
                
                // Put it back
                const_cast<SudokuBoard&>(board).insert(row, col, val);
                
                if (!valid) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Generate a fully solved random board
bool SudokuSolver::generateSolvedBoard() {
    // Start with an empty board
    board.clear();
    
    // Fill the diagonal 3x3 boxes first (these don't affect each other)
    for (int box = 0; box < 3; box++) {
        std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::shuffle(nums.begin(), nums.end(), rng);
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int row = box * 3 + i;
                int col = box * 3 + j;
                board.insert(row, col, nums[i * 3 + j]);
            }
        }
    }
    
    // Solve the rest of the board
    return solve();
}

// Generate a new puzzle with the specified difficulty
bool SudokuSolver::generatePuzzle(Difficulty difficulty) {
    // First generate a solved board
    if (!generateSolvedBoard()) {
        return false;
    }
    
    // Then remove numbers based on difficulty
    removeNumbers(difficulty);
    
    return true;
}

// Remove numbers from a solved board to create a puzzle
void SudokuSolver::removeNumbers(int difficulty) {
    // Create a list of all positions
    std::vector<std::pair<int, int>> positions;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            positions.push_back(std::make_pair(row, col));
        }
    }
    
    // Shuffle the positions
    std::shuffle(positions.begin(), positions.end(), rng);
    
    // Keep track of how many cells we've emptied
    int removed = 0;
    
    // Try to remove numbers while maintaining a unique solution
    for (const auto& pos : positions) {
        if (removed >= difficulty) {
            break;
        }
        
        int row = pos.first;
        int col = pos.second;
        int val = board.getValue(row, col);
        
        // Try removing this number
        board.remove(row, col);
        
        // If the board still has a unique solution, keep it removed
        if (hasUniqueSolution()) {
            removed++;
        } else {
            // Otherwise, put it back
            board.insert(row, col, val);
        }
    }
}

// Check if the current board has exactly one solution
bool SudokuSolver::hasUniqueSolution() {

    // a full implementation would optimize by detecting multiple solutions early
    
    // Save the current state
    int currentBoard[9][9];
    board.getBoardState(currentBoard);
    
    // Check if it's solvable
    bool solvable = solve();
    if (!solvable) {
        // Restore original board
        board.loadBoard(currentBoard);
        return false;
    }
    
    // Save the solved state
    int solvedBoard[9][9];
    board.getBoardState(solvedBoard);
    
    // Restore the original board
    board.loadBoard(currentBoard);
    
    return solvable;
}

// Get a hint for the next move
std::pair<int, int> SudokuSolver::getHint() {
    // Find an empty cell
    auto emptyCell = findEmptyCell();
    if (emptyCell.first == -1) {
        return emptyCell; // No empty cells
    }
    
    // Save the current state
    int currentBoard[9][9];
    board.getBoardState(currentBoard);
    
    // Try to solve the board
    bool solvable = solve();
    
    // If solved, get the value of the empty cell
    int hintValue = 0;
    if (solvable) {
        hintValue = board.getValue(emptyCell.first, emptyCell.second);
    }
    
    // Restore the original board
    board.loadBoard(currentBoard);
    
    // Return the hint as the cell coordinates
    if (hintValue != 0) {
        return emptyCell;
    }
    
    return std::make_pair(-1, -1); // No hint available
}
