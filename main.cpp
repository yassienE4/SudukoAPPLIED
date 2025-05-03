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
        // In main.cpp, inside the game loop
        player gamePlayer;
        gamePlayer.startgame(1); // Start with Easy difficulty
        SudokuBoard* sudoku = gamePlayer.getBoard();
        sudoku->printBoard();

        // std::atomic<bool> timerRunning(true);
        // std::thread timerThread(startTimer, std::ref(timerRunning));

        int row, col, val;
        std::string input;

        while (true) {
            // Display score alongside the prompt
            if (gamePlayer.getScore() <= 0) {
                cout << "Game Over! Your score is Zero.\n";
                break;
            }
            cout << "\nScore: " << gamePlayer.getScore() << " | Enter move (row col value), 'hint', or 'exit': ";
            cin >> input;

            if (input == "exit") break;

            // In main.cpp, update the hint display section
            if (input == "hint") {
                // Get a hint from the solver
                std::pair<int, int> hintCell = gamePlayer.getHint();
                if (hintCell.first != -1) {
                    unorderedSet domain = sudoku->calculateDomain(hintCell.first, hintCell.second);
                    cout << "Hint: Look at position (" << hintCell.first << ", " << hintCell.second << ")\n";
                    cout << "Possible values: ";
                    for (auto it = domain.begin(); it != domain.end(); ++it) {
                        cout << *it << " ";
                    }
                    cout << "\n";
                    gamePlayer.incrementHintCount(); // Reduce score for using hint
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
                // cout << "Move accepted.\n";

                // Check for win after each move
                if (gamePlayer.checkwin()) {
                    cout << "\nCongratulations! You solved the puzzle!\n";
                    cout << "Final Score: " << gamePlayer.getScore() << "\n";
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

    }
    else {
        cout << "Invalid choice. Exiting.\n";
    }

    return 0;
}
