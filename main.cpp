#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <player.h>
#include <viewer.h>
#include <QApplication>
#include <mainwindow.h>

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

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    /*

    if (input == "hint") {


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
    }*/

    return a.exec();
}
