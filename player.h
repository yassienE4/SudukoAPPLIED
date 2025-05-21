//
// Created by Yassien Ebrahim  on 02/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "SudokuSolver.h"
#include <vector>
#include <chrono>
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
    void setDifficulty(int difficulty);
    int getDifficulty() const;
    int getMoveCount() const;
    int getHintCount() const;
    void remove(int row, int col);
    int getScore() const;
    void incrementHintCount();
    void endGame();  // to stop the timer
    int getElapsedTime() const;  // returns seconds
    // Add to player.h
    std::pair<int, int> getHint();

    
    // Move management
    void move(int row, int col, int value);
    void undo();
    
    // Board information
    SudokuBoard* getBoard() const;
    bool isOriginalCell(int row, int col) const;
    int moveCount = 0;
private:
    SudokuSolver* solver;
    SudokuBoard* board;
    SudokuBoard initialBoard; // Stores the initial state of the board
    std::vector<Move> playerMoves; // Tracks moves for undo functionality

    int hintCount = 0;
    int currentDifficulty = 1;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

};

#endif //PLAYER_H
