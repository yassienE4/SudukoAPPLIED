#include "viewer.h"
#include <QGraphicsPixmapItem>;

viewer::viewer(int d, QWidget *parent) :
    QMainWindow(parent)
{
    initUI();
    this->setFixedSize(350,350);
    setdifficulity(d);


    //game
    gamePlayer.startgame(diff);
    SudokuBoard* sudoku = gamePlayer.getBoard();
    loadboard();


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &viewer::update);
    timer->start(16);


}

viewer::~viewer()
{

}

void viewer::update()
{

    score->setText(QString::fromStdString(to_string(gamePlayer.getScore())));

    int elapsed = gamePlayer.getElapsedTime();
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    string t = to_string(mins) + ":" + to_string(secs);
    time->setText(QString::fromStdString(t));
}

void viewer::initUI()
{

    labellist.resize(9, vector<QLineEdit*>(9, nullptr));
    int startX = 50;  // Starting X position
    int startY = 50;  // Starting Y position
    int spacing = 25;  // Space between labels
    int blockSpacing = 10;  // Space between blocks
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            labellist[i][j] = new QLineEdit();
            int x = startX + j * spacing + (j / 3) * blockSpacing;
            int y = startY + i * spacing + (i / 3) * blockSpacing;
            labellist[i][j]->move(x, y);
            labellist[i][j]->setFixedSize(25, 25);
            labellist[i][j]->setParent(this);
            labellist[i][j]->setText("0");
        }
    }

    submitmoves = new QPushButton();
    submitmoves->setText("Submit Moves");
    submitmoves->move(25,0);
    submitmoves->setFixedSize(135,40);
    submitmoves->setParent(this);
    connect(submitmoves, &QPushButton::clicked, this, &viewer::submitpress);

    gethint = new QPushButton();
    gethint->setText("Get a Hint (3/3 Left)");
    gethint->move(190,0);
    gethint->setFixedSize(135,40);
    gethint->setParent(this);
    connect(submitmoves, &QPushButton::clicked, this, &viewer::gethintpress);

    back = new QPushButton();
    back->setText("Go Back");
    back->move(225,300);
    back->setFixedSize(100,40);
    back->setParent(this);
    connect(submitmoves, &QPushButton::clicked, this, &viewer::backpress);

    time = new QLabel();
    time->setText("Time");
    time->move(25,285);
    time->setFixedSize(100,40);
    time->setParent(this);

    score = new QLabel();
    score->setText("Score");
    score->move(25,315);
    score->setFixedSize(100,40);
    score->setParent(this);
}

void viewer::submitpress()
{

}

void viewer::gethintpress()
{

}

void viewer::backpress()
{

}

void viewer::loadboard()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int cell = gamePlayer.getBoard()->board[i][j];
            if(cell != 0)
            {
                labellist[i][j]->setText(QString::fromStdString(to_string(cell)));
                labellist[i][j]->setDisabled(true);
            }


        }
    }
}

void viewer::setdifficulity(int x)
{
    diff = x;
}
