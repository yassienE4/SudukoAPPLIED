#ifndef VIEWER_H
#define VIEWER_H
#include <QMainWindow>
#include <vector>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

#include <sudokuboard.h>
#include <player.h>
using namespace std;

class viewer : public QMainWindow
{
    Q_OBJECT
public:
    viewer(int diff, QWidget *parent = nullptr);
    ~viewer();

    void setdifficulity(int x);
    int diff;
    void loadboard();
    void savemove();
    vector<pair<int,int>> positions;

    //update
    QTimer *timer;
    void update();

    int maxHints =0;
    // ui functions

    void gethintpress();
    void backpress();

    void showWinnerPopup(bool x);

    void initUI();
    // ui elements
    vector<vector<QLineEdit*>> labellist;

    QPushButton *gethint;

    QLabel* time;
    QLabel* score;
    QPushButton *back;

    // game functions
    player gamePlayer;

};

#endif // VIEWER_H
