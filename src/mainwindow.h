#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <viewer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    viewer *v;

private slots:
    void on_EasyButton_clicked();

    void on_MediumButton_clicked();

    void on_HardButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
