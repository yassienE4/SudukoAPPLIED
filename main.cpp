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

    return a.exec();
}
