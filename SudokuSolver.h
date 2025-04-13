//
// Created by Yassien Ebrahim  on 13/04/2025.
//

#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include "sudokuboard.h"
#include <vector>
#include <utility>
#include <random>
#include <ctime>
#include "data-structures/unorderedSet.cpp"

class SudokuSolver {
private:
    SudokuBoard& board;
    std::mt19937 rng; 
    
    // Helper fucntions for solving
    bool solveRecursive(int row, int col);
    std::pair<int, int> findEmptyCell() const;
    bool isValidMove(int row, int col, int val) const;
    
    // helper fucntions
    void removeNumbers(int difficulty);
    bool hasUniqueSolution();
    
public:
    // Difficulty levels
    enum Difficulty {
        EASY = 35,    // 35 empty cells
        MEDIUM = 45,  // 45 empty cells
        HARD = 55,    // 55 empty cells
        EXPERT = 64   // 64 empty cells
    };
    
    // Constructor takes a reference to an existing SudokuBoard
    explicit SudokuSolver(SudokuBoard& sudokuBoard);
    
    // Solve the current board
    bool solve();
    
    // Check if the board is solvable
    bool isSolvable();
    
    // Check if the current board configuration is valid
    bool isValidBoard() const;
    
    // Generate a new random solved board
    bool generateSolvedBoard();
    
    // Generate a new puzzle with the specified difficulty
    bool generatePuzzle(Difficulty difficulty);
    
    // Get a hint (returns a valid move)
    std::pair<int, int> getHint();
};

#endif //SUDOKUSOLVER_H
