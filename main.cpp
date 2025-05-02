#include "SudokuBoard.h"
#include <iostream>
#include <chrono>
#include <thread> //For update frequency

//To display the time in minutes and seconds format
void displayTime(int seconds) {
    int mins = seconds / 60; //Calculate minutes
    int secs = seconds % 60; //Remainder (seconds)
    std::cout << "\rTime: " << mins << ":" << (secs < 10 ? "0" : "") << secs << "   " << std::flush; //Used flush to ensure output is immediately displayed
}

using namespace std;

int main() {
    SudokuBoard sudoku;

    // Initial Sudoku puzzle (0 means empty)
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

    // Load and print initial board
    sudoku.loadBoard(exampleBoard);
    cout << "Initial Sudoku Board:\n";
    sudoku.printBoard();

    // Insert a value and test domain updates
    int testRow = 0, testCol = 2, testValue = 4;
    cout << "\nInserting value " << testValue << " at (" << testRow << ", " << testCol << ")\n";

    if (sudoku.insert(testRow, testCol, testValue)) {
        cout << "Inserted successfully.\n";
    } else {
        cout << "Failed to insert value.\n";
    }

    cout << "\nBoard after insertion:\n";
    sudoku.printBoard();

    // Print domain of a related cell
    int queryRow = 3, queryCol = 3;
    cout << "\nDomain of (" << queryRow << ", " << queryCol << ") after insertion: ";
    unorderedSet domain = sudoku.calculateDomain(queryRow, queryCol);

    for (auto it = domain.begin(); it != domain.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Remove the previously inserted value
    cout << "\nRemoving value at (" << testRow << ", " << testCol << ")\n";
    if (sudoku.remove(testRow, testCol)) {
        cout << "Removed successfully.\n";
    } else {
        cout << "Failed to remove value.\n";
    }

    cout << "\nBoard after removal:\n";
    sudoku.printBoard();

    cout << "\nDomain of (" << queryRow << ", " << queryCol << ") after removal: ";
    domain = sudoku.calculateDomain(queryRow, queryCol);
    for (auto it = domain.begin(); it != domain.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Clear board
    cout << "\nClearing board...\n";
    sudoku.clear();

    cout << "Board after clearing:\n";
    sudoku.printBoard();

    // Reload original board
    cout << "\nReloading original puzzle...\n";
    sudoku.loadBoard(exampleBoard);
    cout << "Reloaded board:\n";
    sudoku.printBoard();

    // Timer
    auto start = std::chrono::steady_clock::now(); //Record start time
    int elapsed_seconds = 0; //Initialize variable to store the time in seconds
    bool gameOver = false;

    while (!gameOver) {
        auto now = std::chrono::steady_clock::now(); //Current time
        elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start).count(); //Calculate the elapsed time in seconds by subtracting the start time from the current time
        displayTime(elapsed_seconds); //Use display time function to show the elapsed time in minutes and seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Update every 100ms
    }

    return 0;
}
