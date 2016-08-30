#include "connectdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(Const::NetworkIdentity type, const QString& ip, int port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_type(type), m_ip(ip), m_port(port)
{
    ui->setupUi(this);
    createConnection();

    m_is_block = true;
    ui->board->SetBlock(true);
    ui->label->setText(tr("Waiting for connection..."));

    if (m_type == Const::Server)
        m_player = new Player(Pieces::White);
    else
        m_player = new Player(Pieces::Black);

    ui->board->SetColor(m_player->Type());

    connect(ui->board, &Board::piecePlaced, this, &MainWindow::onPiecePlaced);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createConnection()
{
    switch (m_type)
    {
    case Const::Server:
        m_server = new QTcpServer(this);
        m_server->listen(QHostAddress(m_ip), m_port);
        connect(m_server, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);
        break;
    case Const::Client:
        m_socket = new QTcpSocket(this);
        connect(m_socket, SLOT(error(QAbstractSocket::SocketError)), this, SIGNAL(onConnectionError(QAbstractSocket::SocketError)));
        connect(m_socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
        connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::onDisConnected);
        connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReceivedData);
        m_socket->connectToHost(QHostAddress(m_ip), m_port);
        break;
    }
}

void MainWindow::onNewConnection()
{
    qDebug()<<"New Connection";
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReceivedData);

    m_is_block = false;
    ui->board->SetBlock(false);
    ui->label->setText(tr("Please select a position to place the pieces"));
}

void MainWindow::onConnectionError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"ConnectionError";
}

void MainWindow::onConnected()
{
    qDebug()<<"Connected";
    ui->label->setText(tr("Waiting for the opponent to place..."));
}

void MainWindow::onDisConnected()
{
    qDebug()<<"DisConnected";
}

void MainWindow::onReceivedData()
{
    QDataStream in(m_socket);
    int row, col, color_int;
    in >> row >> col >> color_int;
    ui->board->PlacePiece(row, col, (Pieces::PiecesColor)color_int);

    m_is_block = true;
    ui->board->SetBlock(false);
    ui->label->setText(tr("Please select a position to place the pieces"));
}

void MainWindow::onPiecePlaced(int row, int col, Pieces::PiecesColor color)
{
    m_is_block = false;
    ui->board->SetBlock(true);
    ui->label->setText(tr("Waiting for the opponent to place..."));

    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << row << col << (int)color;
    m_socket->write(array);
}
