#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->board->PlacePiece(3, 4, Pieces::White);
    ui->board->PlacePiece(2, 3, Pieces::Black);
}

MainWindow::~MainWindow()
{
    delete ui;
}
