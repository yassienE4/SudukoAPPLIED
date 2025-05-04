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
    
    // First check if the board is completely filled
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board->getValue(row, col) == 0) {
                return false; // Empty cell found, puzzle not complete
            }
        }
    }

    // Then check if it's valid according to Sudoku rules
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
    currentDifficulty = difficulty;

    
    // Generate a puzzle with the specified difficulty
    solver->generatePuzzle(diff);
    
    // Get the board state and copy it
    int boardState[9][9];
    solver->getBoard().getBoardState(boardState);
    board->loadBoard(boardState);
    
    // Store the initial board state
    initialBoard = *board;
    
    // Initialize player move history
    playerMoves.clear();
    moveCount = 0;
    hintCount = 0;
    startTime = std::chrono::steady_clock::now();

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

void player::move(int row, int col, int value) {
    if (board == nullptr) {
        return;
    }

    // Check if cell is editable (not part of the original puzzle)
    if (isOriginalCell(row, col)) {
        std::cout << "Cannot modify original cell at (" << row << ", " << col << ")." << std::endl;
        return; // Can't modify original cells
    }

    // Validate move according to Sudoku rules
    // we will remove this for the gui so that the user can actually make mistakes

    unorderedSet domain = board->calculateDomain(row, col);
    if (!domain.contains(value)) {
        std::cout<< "Invalid move: " << value << " is not a valid option for cell (" << row << ", " << col << ")." << std::endl;
        moveCount++;

        return; // Invalid move
    }

    // Store the move for potential undo
    Move newMove = {row, col, board->getValue(row, col), value};
    playerMoves.push_back(newMove);
    std::cout<< "Move made: " << value << " at (" << row << ", " << col << ")." << std::endl;

    // Update the board
    board->insert(row, col, value);
    moveCount++;
}

void player::incrementHintCount() {
    hintCount++;
}
void player::endGame() {
    endTime = std::chrono::steady_clock::now();
}
int player::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    // If endTime is after startTime, use endTime (game over); otherwise, use now (game running)
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        (endTime > startTime ? endTime : now) - startTime
    );
    return static_cast<int>(duration.count());
}

// Update in player.cpp
int player::getScore() const {
    // Calculate time penalty (seconds elapsed)
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        endTime.time_since_epoch() > startTime.time_since_epoch() ?
        endTime - startTime : now - startTime);
    int timePenalty = static_cast<int>(duration.count()) / 10; // 1 point per 10 seconds
    int movePenalty = moveCount * 5;      // 5 points per move
    int hintPenalty = hintCount * 50;     // 50 points per hint

    // Set base score by difficulty
    int baseScore;
    switch (currentDifficulty) {
        case 1: // Easy
            baseScore = 2000;
            timePenalty = static_cast<int>(duration.count()) / 10; // 1 point per 10 seconds
            movePenalty = moveCount * 2;      // 2 points per move
            hintPenalty = hintCount * 20;     // 20 points per hint
        break;
        case 2: // Medium
            baseScore = 1500;
            timePenalty = static_cast<int>(duration.count()) / 10; // 1 point per 10 seconds
            movePenalty = moveCount * 3;      // 3 points per move
            hintPenalty = hintCount * 50;     // 50 points per hint
        break;
        case 3: // Hard
            baseScore = 1000;
            timePenalty = static_cast<int>(duration.count()) / 5; // 1 point per 5 seconds
            movePenalty = moveCount * 5;      // 5 points per move
            hintPenalty = hintCount * 100;     // 100 points per hint
        break;
        default:
            baseScore = 2000;
    }



    int score = baseScore - movePenalty - hintPenalty - timePenalty;
    return std::max(score, 0); // Score can't go below 0
}




void player::undo()
{
    if (!playerMoves.empty() && board != nullptr) {
        Move lastMove = playerMoves.back();
        playerMoves.pop_back();
        moveCount++;
        
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

std::pair<int, int> player::getHint() {
    if (board == nullptr) {
        return std::make_pair(-1, -1);
    }

    int minDomainSize = 10; // More than maximum possible (9)
    std::pair<int, int> bestCell(-1, -1);

    // Search for the empty cell with the smallest domain
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board->isEmpty(row, col)) {
                unorderedSet domain = board->calculateDomain(row, col);
                int domainSize = domain.size();

                // If we find a cell with domain size 1, that's an immediate hint
                if (domainSize == 1) {
                    return std::make_pair(row, col);
                }

                // Otherwise keep track of the cell with smallest domain
                if (domainSize < minDomainSize && domainSize > 0) {
                    minDomainSize = domainSize;
                    bestCell = std::make_pair(row, col);
                }
            }
        }
    }

    // If we found a cell with a constrained domain
    if (bestCell.first != -1) {
        return bestCell;
    }

    // Fallback to finding any empty cell
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board->isEmpty(row, col)) {
                return std::make_pair(row, col);
            }
        }
    }

    // No empty cells found
    return std::make_pair(-1, -1);
}


void player::setDifficulty(int difficulty) {
    currentDifficulty = difficulty;
}
int player::getDifficulty() const {
    return currentDifficulty;
}

int player::getMoveCount() const {
    return moveCount;
}
int player::getHintCount() const {
    return hintCount;
}

void player::remove(int row, int col) {
    if (board == nullptr) return;
    if (isOriginalCell(row, col)) {
        std::cout << "Cannot remove value from a fixed/original cell.\n";
        return;
    }
    int prevValue = board->getValue(row, col);
    if (prevValue == 0) {
        std::cout << "Cell (" << row << ", " << col << ") is already empty.\n";
        return;
    }
    // Optionally store for redo/undo
    Move newMove = {row, col, prevValue, 0};
    playerMoves.push_back(newMove);
    board->remove(row, col);
    moveCount++;
}
