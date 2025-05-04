#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <player.h>

#include "sudokuBoard.h"
#include "data-structures/unorderedSet.h"

using namespace std;

// Display timer (runs in separate thread, prints in-place)
void startTimer(std::atomic<bool>& running) {
    auto start = std::chrono::steady_clock::now();
    while (running.load()) {
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        int mins = elapsed / 60;
        int secs = elapsed % 60;

        // \r returns to the beginning of the line; std::flush ensures it's immediately printed
        std::cout << "\rTime Elapsed: " << mins << ":" << (secs < 10 ? "0" : "") << secs << "   " << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Clean up line after exit
    std::cout << "\rTime Elapsed: --:--           \n";
}

int main() {
    SudokuBoard sudoku;

    int exampleBoard[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    cout << "Choose mode:\n1. Play Sudoku\n2. Run Example\nChoice: ";
    int choice;
    cin >> choice;

    if (choice == 2) {
        sudoku.loadBoard(exampleBoard);
        cout << "Initial Sudoku Board:\n";
        sudoku.printBoard();

        int testRow = 0, testCol = 2, testValue = 4;
        cout << "\nInserting value " << testValue << " at (" << testRow << ", " << testCol << ")\n";
        if (sudoku.insert(testRow, testCol, testValue)) {
            cout << "Inserted successfully.\n";
        } else {
            cout << "Failed to insert value.\n";
        }

        cout << "\nBoard after insertion:\n";
        sudoku.printBoard();

        int queryRow = 0, queryCol = 2;
        cout << "\nDomain of (" << queryRow << ", " << queryCol << "): ";
        unorderedSet domain = sudoku.calculateDomain(queryRow, queryCol);
        for (auto it = domain.begin(); it != domain.end(); ++it)
            cout << *it << " ";
        cout << "\n";

        cout << "\nRemoving value at (" << testRow << ", " << testCol << ")\n";
        sudoku.remove(testRow, testCol);
        sudoku.printBoard();

        cout << "\nClearing board...\n";
        sudoku.clear();
        sudoku.printBoard();

        cout << "\nReloading original board...\n";
        sudoku.loadBoard(exampleBoard);
        sudoku.printBoard();
    }
    else if (choice == 1) {
        //ask for difficulty level
        int difficulty = 1;
        cout << "Select difficulty level (1: Easy, 2: Medium, 3: Hard): ";
        cin >> difficulty;
        if (difficulty < 1 || difficulty > 3) {
            cout << "Invalid difficulty level. Exiting.\n";
            return 1;
        }
        player gamePlayer;
        gamePlayer.startgame(difficulty); // Start with Easy difficulty
        SudokuBoard* sudoku = gamePlayer.getBoard();
        sudoku->printBoard();


        int row, col, val;
        std::string input;

        while (true) {
            // Calculate elapsed time up to now
            int elapsed = gamePlayer.getElapsedTime();
            int mins = elapsed / 60;
            int secs = elapsed % 60;

            // Set max hints based on difficulty
            int maxHints;
            switch (gamePlayer.getDifficulty()) {
                case 1: maxHints = 10; break; // Easy
                case 2: maxHints = 5; break;  // Medium
                case 3: maxHints = 3; break;  // Hard
                default: maxHints = 0; break;
            }

            // Display all info in a single prompt line
            cout << "\rTime: " << mins << ":" << (secs < 10 ? "0" : "") << secs
                 << " | Score: " << gamePlayer.getScore()
                 << " | Moves: " << gamePlayer.getMoveCount()
                 << " | Hints: " << gamePlayer.getHintCount() << "/" << maxHints
                 << " | Enter move (row col value), 'hint', 'remove', 'undo', or 'exit': " << flush;

            cin >> input;

    if (input == "exit") {
        gamePlayer.endGame();
        break;
    }

    if (input == "undo") {
        gamePlayer.undo();
        sudoku->printBoard();
        continue;
    }
    if (input == "remove") {
        int remRow, remCol;
        cout << "Enter row and column to remove value (0-based): ";
        cin >> remRow >> remCol;
        if (gamePlayer.isOriginalCell(remRow, remCol)) {
            cout << "Cannot remove value from a fixed/original cell.\n";
        } else {
            gamePlayer.remove(remRow, remCol);
            cout << "Value removed from (" << remRow << ", " << remCol << ").\n";
        }
        sudoku->printBoard();
        continue;
    }

    if (input == "hint") {
        //vary max hints based on difficulty
        // Easy: 3 hints, Medium: 2 hints, Hard: 1 hint

        if (gamePlayer.getHintCount() >= maxHints) {
            cout << "No more hints available.\n";
            continue;
        }
        std::pair<int, int> hintCell = gamePlayer.getHint();
        if (hintCell.first != -1 ) {
            unorderedSet domain = sudoku->calculateDomain(hintCell.first, hintCell.second);
            cout << "Hint: Look at position (" << hintCell.first << ", " << hintCell.second << ")\n";
            cout << "Possible values: ";
            for (auto it = domain.begin(); it != domain.end(); ++it) {
                cout << *it << " ";
            }
            cout << "\n";
            gamePlayer.incrementHintCount();
        } else {
            cout << "No hint available.\n";
        }
        continue;
    }

    // Parse regular move input
    try {
        row = std::stoi(input);
        cin >> col >> val;

        if (gamePlayer.isOriginalCell(row, col)) {
            cout << "Cannot change a fixed cell.\n";
            continue;
        }

        if (val < 1 || val > 9 || row < 0 || row > 8 || col < 0 || col > 8) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        gamePlayer.move(row, col, val);

        // Check for win after each move
        if (gamePlayer.checkwin()) {
            cout << "\nCongratulations! You solved the puzzle!\n";
            gamePlayer.endGame();
            break;
        }
    } catch (std::invalid_argument&) {
        cout << "Invalid input. Try again.\n";
    }

    sudoku->printBoard();
}


        // timerRunning = false;
        // timerThread.join();
        cout << "\nGame over. Thanks for playing!\n";
        cout << "Final Score: " << gamePlayer.getScore() << endl;
        cout << "Total Moves: " << gamePlayer.getMoveCount() << endl;
        cout << "Hints Used: " << gamePlayer.getHintCount() << endl;
        int elapsed = gamePlayer.getElapsedTime();
        int mins = elapsed / 60;
        int secs = elapsed % 60;
        cout << "Time Elapsed: " << mins << ":" << (secs < 10 ? "0" : "") << secs << endl;

    }
    else {
        cout << "Invalid choice. Exiting.\n";
    }

    return 0;
}
