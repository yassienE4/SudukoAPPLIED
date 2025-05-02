//
// Created by Yassien Ebrahim  on 02/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "SudokuSolver.h"

class player 
{
  public:
    player();
    ~player();
    bool checkwin();
    void startgame(int difficulty);
    void restart();
    void move();
    private:
        SudokuSolver* solver;
        SudokuBoard* board;
};



#endif //PLAYER_H
