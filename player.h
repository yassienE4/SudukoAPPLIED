//
// Created by Yassien Ebrahim  on 02/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "SudokuSolver.h"
#include <vector>

// Structure to store move information for undo functionality
struct Move {
    int row;
    int col;
    int previousValue;
    int newValue;
};

class player 
{
public:
    player();
    ~player();
    
    // Game management
    void startgame(int difficulty); // Uses SudokuSolver's difficulty system
    void restart();
    bool checkwin();
    
    // Move management
    void move(int row, int col, int value);
    void undo();
    
    // Board information
    SudokuBoard* getBoard() const;
    bool isOriginalCell(int row, int col) const;
    
private:
    SudokuSolver* solver;
    SudokuBoard* board;
    SudokuBoard initialBoard; // Stores the initial state of the board
    std::vector<Move> playerMoves; // Tracks moves for undo functionality
};

#endif //PLAYER_H
