#include "viewer.h"
#include <QGraphicsPixmapItem>
#include <QDialog>
#include <QVBoxLayout>


viewer::viewer(int d, QWidget *parent) :
    QMainWindow(parent)
{
    setdifficulity(d);
    initUI();
    this->setFixedSize(350,350);


    //game
    gamePlayer.startgame(diff);
    SudokuBoard* sudoku = gamePlayer.getBoard();
    loadboard();


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &viewer::update);
    timer->start(1000);


}

viewer::~viewer()
{

}

void viewer::update()
{
    savemove();
    score->setText(QString::fromStdString(to_string(gamePlayer.getScore())));
    int elapsed = gamePlayer.getElapsedTime();
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    string t = to_string(mins) + ":" + to_string(secs);
    time->setText(QString::fromStdString(t));
}



void viewer::gethintpress()
{
    if (gamePlayer.getHintCount() >= maxHints)
    {
        gethint->setDisabled(true);
    }

    pair<int, int> hintCell = gamePlayer.getHint();
    if (hintCell.first != -1 )
    {
        unorderedSet domain = gamePlayer.getBoard()->calculateDomain(hintCell.first, hintCell.second);
        int value;
        for (int b : domain) {
            value = b;
        }

        labellist[hintCell.first][hintCell.second]->setText( QString::number(value));
        labellist[hintCell.first][hintCell.second]->setStyleSheet("background-color: green;");
        gamePlayer.incrementHintCount();
        int hintsleft = gamePlayer.getHintCount();
        gethint->setText("Get a Hint ("+QString::number(hintsleft)+"/"+QString::number(maxHints)+" Left)");
    }
}

void viewer::backpress()
{
    this->hide();
    this->parentWidget()->show();
}

void viewer::savemove()
{
    static int won = 1;
    for(auto pos : positions)
    {
        if(labellist[pos.first][pos.second]->text() != "0" && labellist[pos.first][pos.second]->text() != "")
        {
            int val = labellist[pos.first][pos.second]->text().toInt();
            if (val < 1 || val > 9)
            {
                labellist[pos.first][pos.second]->setText("0");
            }
            else
            {
                int cell = gamePlayer.getBoard()->board[pos.first][pos.second];
                if(val != cell)
                {
                    unorderedSet domain = gamePlayer.getBoard()->calculateDomain(pos.first, pos.second);
                    if (!domain.contains(val))
                    {
                        labellist[pos.first][pos.second]->setText("0");
                        gamePlayer.moveCount++;
                    }
                    else
                    {
                        gamePlayer.move(pos.first, pos.second, val);
                    }

                }
                if(gamePlayer.checkwin())
                {
                    if(won == 1)
                    {
                        showWinnerPopup();
                        won++;
                    }
                }
            }
        }
    }
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
            else
            {
                positions.push_back(make_pair(i,j));
            }
        }
    }
}

void viewer::setdifficulity(int x)
{
    diff = x;
    switch (diff) {
    case 1: maxHints = 10; break; // Easy
    case 2: maxHints = 5; break;  // Medium
    case 3: maxHints = 3; break;  // Hard
    default: maxHints = 0; break;
    }
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

    gethint = new QPushButton();
    QString mh = QString::number(maxHints);
    gethint->setText("Get a Hint (0/"+mh+" Left)");
    gethint->move(100,0);
    gethint->setFixedSize(135,40);
    gethint->setParent(this);
    connect(gethint, &QPushButton::clicked, this, &viewer::gethintpress);

    back = new QPushButton();
    back->setText("Go Back");
    back->move(225,300);
    back->setFixedSize(100,40);
    back->setParent(this);
    connect(back, &QPushButton::clicked, this, &viewer::backpress);

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

void viewer::showWinnerPopup()
{

    QString winstring = "You Won!";
    QDialog popup(this);
    popup.setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    popup.setFixedSize(100, 100);

    QVBoxLayout layout(&popup);
    QLabel label(winstring, &popup);
    label.setAlignment(Qt::AlignCenter);

    QPushButton closeButton("Close", &popup);
    connect(&closeButton, &QPushButton::clicked, &popup, &QDialog::accept);

    layout.addWidget(&label);
    layout.addWidget(&closeButton);
    layout.setAlignment(Qt::AlignCenter);

    popup.exec(); // Modal dialog (use popup.show() for non-modal)
}
