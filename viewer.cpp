#include "viewer.h"
#include <QGraphicsPixmapItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <set>

viewer::viewer(int d, QWidget *parent) :
    QMainWindow(parent)
{
    setdifficulity(d);
    initUI();
    this->setFixedSize(500, 500);

    //game
    gamePlayer.startgame(diff);
    loadboard();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &viewer::update);
    timer->start(500);
}

viewer::~viewer()
{
    for(int i = 0; i < labellist.size(); i++)
    {
        for(int j = 0; j < labellist[i].size(); j++)
        {
            delete labellist[i][j];
        }
    }
    delete timer;
    delete gethint;
    delete time;
    delete score;
    delete back;
    delete penModeButton;
    delete clearPenButton;
}

void viewer::update()
{
    static int shown = 0;
    if(gamePlayer.getScore() == 0 && shown == 0)
    {
        gamePlayer.endGame();
        shown++;
        showWinnerPopup(0);
    }
    else
    {
        savemove();
        score->setText(QString::fromStdString(to_string(gamePlayer.getScore())));
        int elapsed = gamePlayer.getElapsedTime();
        int mins = elapsed / 60;
        int secs = elapsed % 60;
        string t = to_string(mins) + ":" + (secs < 10 ? "0" : "") + to_string(secs);
        time->setText(QString::fromStdString(t));
    }
}

void viewer::gethintpress()
{
    if (gamePlayer.getHintCount() >= maxHints-1)
    {
        gethint->setDisabled(true);
    }

    pair<int, int> hintCell = gamePlayer.getHint();
    if (hintCell.first != -1)
    {
        unorderedSet domain = gamePlayer.getBoard()->calculateDomain(hintCell.first, hintCell.second);
        int value = 0;
        for (int b : domain) {
            value = b;
            break; // Take the first valid value
        }

        if (value > 0) {
            // Clear any pen marks for this cell
            clearPenMarks(hintCell.first, hintCell.second);

            labellist[hintCell.first][hintCell.second]->setText(QString::number(value));
            labellist[hintCell.first][hintCell.second]->setStyleSheet("background-color: green;");
            gamePlayer.incrementHintCount();
            int hintsleft = gamePlayer.getHintCount();
            gethint->setText("Get a Hint ("+QString::number(hintsleft)+"/"+QString::number(maxHints)+" Used)");
        }
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

    // Don't process moves in pen mode
    if (isPenMode) return;

    for(auto pos : positions)
    {
        if(labellist[pos.first][pos.second]->text() == "")
        {
            // If the cell is empty, remove the value from the game model
            int currentValue = gamePlayer.getBoard()->getValue(pos.first, pos.second);
            if(currentValue != 0) {
                gamePlayer.remove(pos.first, pos.second);
                clearPenMarks(pos.first, pos.second);
            }
        }
        else if(labellist[pos.first][pos.second]->text() != "0")
        {
            // Only process if the text is a single digit (not pen marks)
            QString cellText = labellist[pos.first][pos.second]->text();
            if (cellText.length() == 1) {
                int val = cellText.toInt();
                if (val < 1 || val > 9)
                {
                    labellist[pos.first][pos.second]->setText("");
                }
                else
                {
                    int cell = gamePlayer.getBoard()->board[pos.first][pos.second];
                    if(val != cell)
                    {
                        unorderedSet domain = gamePlayer.getBoard()->calculateDomain(pos.first, pos.second);
                        if (!domain.contains(val))
                        {
                            labellist[pos.first][pos.second]->setText("");
                            gamePlayer.moveCount++;
                        }
                        else
                        {
                            gamePlayer.move(pos.first, pos.second, val);
                            // Clear any pen marks for this cell after a move
                            clearPenMarks(pos.first, pos.second);
                        }
                    }
                }
            }
        }
    }

    if(gamePlayer.checkwin())
    {
        if(won == 1)
        {
            showWinnerPopup(1);
            won++;
            gamePlayer.endGame();
        }
    }
}

void viewer::clearPenMarks(int row, int col)
{
    auto cellKey = std::make_pair(row, col);
    if (penMarks.find(cellKey) != penMarks.end()) {
        penMarks.erase(cellKey);
        updateCellDisplay(row, col);
    }
}

void viewer::clearAllPenMarks()
{
    penMarks.clear();
    // Update all cells
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            updateCellDisplay(i, j);

        }
    }
}
void viewer::loadboard()
{
    positions.clear(); // Clear previous positions

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
                labellist[i][j]->setText(""); // Initialize empty cells with empty string
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
bool viewer::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        QLineEdit* cell = qobject_cast<QLineEdit*>(obj);

        if (cell) {
            // Find the row and column of this cell
            int row = -1, col = -1;
            for (int i = 0; i < 9 && row == -1; i++) {
                for (int j = 0; j < 9; j++) {
                    if (labellist[i][j] == cell) {
                        row = i;
                        col = j;
                        break;
                    }
                }
            }

            if (row != -1 && !cell->isReadOnly()) {
                int key = keyEvent->key();

                // Handle backspace and delete keys
                if (key == Qt::Key_Backspace || key == Qt::Key_Delete) {
                    if (!isPenMode) {
                        cell->setText("");
                        // Remove the value from the game model
                        gamePlayer.remove(row, col);
                        clearPenMarks(row, col);
                        return true;
                    } else {
                        // In pen mode, clear all pen marks for this cell
                        auto cellKey = std::make_pair(row, col);
                        if (penMarks.find(cellKey) != penMarks.end()) {
                            penMarks.erase(cellKey);
                            updateCellDisplay(row, col);
                        }
                        return true;
                    }
                }

                // Handle number keys
                if (key >= Qt::Key_1 && key <= Qt::Key_9) {
                    int num = key - Qt::Key_0;

                    if (isPenMode) {
                        // Handle pen mode input
                        auto cellKey = std::make_pair(row, col);
                        if (penMarks.find(cellKey) == penMarks.end()) {
                            penMarks[cellKey] = std::set<int>();
                        }

                        // Toggle the pen mark
                        if (penMarks[cellKey].find(num) != penMarks[cellKey].end()) {
                            penMarks[cellKey].erase(num);
                        } else {
                            penMarks[cellKey].insert(num);
                        }

                        updateCellDisplay(row, col);
                        return true;
                    }
                    // Normal mode continues with default behavior
                }
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}


void viewer::initUI()
{
    // Scale up the grid for the larger window
    labellist.resize(9, vector<QLineEdit*>(9, nullptr));
    int startX = 85;  // Increased starting X position
    int startY = 100;  // Increased starting Y position
    int spacing = 35;  // Increased space between cells
    int blockSpacing = 12;  // Increased space between blocks

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            labellist[i][j] = new QLineEdit();
            int x = startX + j * spacing + (j / 3) * blockSpacing;
            int y = startY + i * spacing + (i / 3) * blockSpacing;
            labellist[i][j]->move(x, y);
            labellist[i][j]->setFixedSize(35, 35);  // Larger cells
            labellist[i][j]->setParent(this);
            // labellist[i][j]->setText("0");
            labellist[i][j]->setAlignment(Qt::AlignCenter);  // Center the text
            labellist[i][j]->installEventFilter(this);
        }
    }

    // Top row buttons
    penModeButton = new QPushButton();
    penModeButton->setText("Pen Mode: Off");
    penModeButton->move(20, 20);
    penModeButton->setFixedSize(120, 40);
    penModeButton->setParent(this);
    connect(penModeButton, &QPushButton::clicked, this, &viewer::togglePenMode);

    gethint = new QPushButton();
    QString mh = QString::number(maxHints);
    gethint->setText("Get a Hint (0/"+mh+" Used)");
    gethint->move(150, 20);
    gethint->setFixedSize(180, 40);
    gethint->setParent(this);
    connect(gethint, &QPushButton::clicked, this, &viewer::gethintpress);

    clearPenButton = new QPushButton();
    clearPenButton->setText("Clear Pen");
    clearPenButton->move(340, 20);
    clearPenButton->setFixedSize(140, 40);
    clearPenButton->setParent(this);
    connect(clearPenButton, &QPushButton::clicked, this, &viewer::clearAllPenMarks);

    // Bottom row elements
    time = new QLabel();
    time->setText("Time: 0:00");
    time->move(20, 460);
    time->setFixedSize(150, 40);
    time->setParent(this);
    time->setStyleSheet("QLabel { font-size: 14pt; }");

    score = new QLabel();
    score->setText("Score: 0");
    score->move(210, 460);
    score->setFixedSize(150, 40);
    score->setParent(this);
    score->setStyleSheet("QLabel { font-size: 14pt; }");

    back = new QPushButton();
    back->setText("Go Back");
    back->move(360, 460);
    back->setFixedSize(130, 40);
    back->setParent(this);
    connect(back, &QPushButton::clicked, this, &viewer::backpress);
}

void viewer::togglePenMode()
{
    isPenMode = !isPenMode;
    if (isPenMode) {
        penModeButton->setText("Pen Mode: On");
        penModeButton->setStyleSheet("background-color: red;");
    } else {
        clearAllPenMarks();
        penModeButton->setText("Pen Mode: Off");
        penModeButton->setStyleSheet("");
    }
}
void viewer::updateCellDisplay(int row, int col)
{
    QLineEdit* cell = labellist[row][col];
    auto cellKey = std::make_pair(row, col);

    // Skip fixed cells
    if (cell->isReadOnly()) {
        return;
    }

    if (isPenMode && penMarks.find(cellKey) != penMarks.end() && !penMarks[cellKey].empty()) {
        // Display pen marks in a smaller font
        QString penText = "";
        for (int mark : penMarks[cellKey]) {
            penText += QString::number(mark);
        }
        cell->setText(penText);
        cell->setStyleSheet("color: red; font-size: 10pt;");
    } else {
        // Check if there's an actual value in the game board
        int value = gamePlayer.getBoard()->getValue(row, col);

        // If the cell is marked green due to it being a hint don't remove color
        if (cell->styleSheet().contains("background-color: green")) {
            if (value > 0) {
                cell->setText(QString::number(value));
            } else {
                cell->setText("");
            }
            return;
        }

        if (value > 0) {
            cell->setText(QString::number(value));
            cell->setStyleSheet("");
        } else {
            cell->setText("");
            cell->setStyleSheet("");
        }
    }
}


void viewer::showWinnerPopup(bool x)
{
    QString winstring;
    if(x)
        winstring = "You Won!";
    else
        winstring = "You Lost!";
    QDialog popup(this);
    popup.setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    popup.setFixedSize(200, 150);  // Larger popup

    QVBoxLayout layout(&popup);
    QLabel label(winstring, &popup);
    label.setAlignment(Qt::AlignCenter);
    label.setStyleSheet("font-size: 18pt; font-weight: bold;");

    QPushButton closeButton("Close", &popup);
    closeButton.setFixedSize(100, 40);
    connect(&closeButton, &QPushButton::clicked, &popup, &QDialog::accept);

    layout.addWidget(&label);
    layout.addWidget(&closeButton);
    layout.setAlignment(Qt::AlignCenter);

    popup.exec(); // Modal dialog (use popup.show() for non-modal)
}
