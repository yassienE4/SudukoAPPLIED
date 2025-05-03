//
// Created by Yassien Ebrahim  on 02/05/2025.
//

#include "player.h"
#include <iostream>

player::player()
{
    solver = new SudokuSolver(*new SudokuBoard()); // Based on SudokuSolver constructor that takes a reference
    board = nullptr;
}

player::~player()
{
    delete solver;
    if (board != nullptr) {
        delete board;
    }
}

bool player::checkwin()
{
    if (board == nullptr) {
        return false;
    }
    
    // Check if the board is valid using isValidBoard method seen in SudokuSolver
    return solver->isValidBoard();
}

void player::startgame(int difficulty)
{
    // Clean up previous game if any
    if (board != nullptr) {
        delete board;
    }
    
    // Create a new board
    board = new SudokuBoard();
    
    // Using the SudokuSolver's Difficulty enum seen in the code
    SudokuSolver::Difficulty diff;
    switch (difficulty) {
        case 1:
            diff = SudokuSolver::EASY;
            break;
        case 2:
            diff = SudokuSolver::MEDIUM;
            break;
        case 3:
            diff = SudokuSolver::HARD;
            break;
        default:
            diff = SudokuSolver::MEDIUM;
    }
    
    // Generate a puzzle with the specified difficulty
    solver->generatePuzzle(diff);
    
    // Get the board state and copy it
    int boardState[9][9];
    solver->getBoard()->getBoardState(boardState);
    board->loadBoard(boardState);
    
    // Store the initial board state
    initialBoard = *board;
    
    // Initialize player move history
    playerMoves.clear();
}

void player::restart()
{
    if (board != nullptr) {
        // Reset to initial board state
        int initialBoardState[9][9];
        initialBoard.getBoardState(initialBoardState);
        board->loadBoard(initialBoardState);
        playerMoves.clear();
    }
}

void player::move(int row, int col, int value)
{
    if (board == nullptr) {
        return;
    }
    
    // Check if cell is editable (not part of the original puzzle)
    if (initialBoard.getValue(row, col) == 0) {
        // Store the move for potential undo
        Move newMove = {row, col, board->getValue(row, col), value};
        playerMoves.push_back(newMove);
        
        // Update the board
        board->insert(row, col, value);
    }
}

void player::undo()
{
    if (!playerMoves.empty() && board != nullptr) {
        Move lastMove = playerMoves.back();
        playerMoves.pop_back();
        
        // If previous value was 0, remove the value, otherwise insert the previous value
        if (lastMove.previousValue == 0) {
            board->remove(lastMove.row, lastMove.col);
        } else {
            board->insert(lastMove.row, lastMove.col, lastMove.previousValue);
        }
    }
}

SudokuBoard* player::getBoard() const
{
    return board;
}

bool player::isOriginalCell(int row, int col) const
{
    if (board == nullptr) {
        return false;
    }
    
    // Original cells have non-zero values in the initial board
    return initialBoard.getValue(row, col) != 0;
}
