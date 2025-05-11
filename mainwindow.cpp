#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_EasyButton_clicked()
{
    v = new viewer(1,this);
    this->hide();
    v->show();
}


void MainWindow::on_MediumButton_clicked()
{
    v = new viewer(2,this);
    this->hide();
    v->show();
}


void MainWindow::on_HardButton_clicked()
{
    v = new viewer(3,this);
    this->hide();
    v->show();
}

