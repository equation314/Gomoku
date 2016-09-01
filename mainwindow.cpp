#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "choosecolordialog.h"

#include <QMessageBox>
#include <QPainter>

MainWindow::MainWindow(const QString& username, Const::HostType type, const QString& ip, int port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_username(username), m_type(type), m_ip(ip), m_port(port),
    m_is_block(true), m_is_closing(false), m_is_choosing_color(false), m_is_connected(false),
    m_server(nullptr), m_thread(nullptr)
{
    ui->setupUi(this);

    if (m_type == Const::Server)
    {
        this->setWindowTitle(tr("Gomoku - Sevrer"));
        ui->groupBox_player_2->setEnabled(false);
        m_server = new Server(m_ip, m_port, this);
        connect(m_server, &Server::newConnection, this, &MainWindow::createServerConnection);
    }
    else if (m_type == Const::Client)
    {
        this->setWindowTitle(tr("Gomoku - Client"));
        ui->groupBox_player_1->setEnabled(false);
    }

    connect(ui->board, &Board::piecePlaced, this, &MainWindow::onMyMove);
    connect(ui->board, &Board::gameOver, this, &MainWindow::onGameStartPrepare);

    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_is_closing = true;
    emit disconnected();
    qApp->exit(0);
}

void MainWindow::setBlock(bool isBlock)
{
    m_is_block = isBlock;
    ui->board->SetBlock(isBlock);
    ui->pushButton_hint->setEnabled(!isBlock);
    ui->pushButton_back->setEnabled(!isBlock);
}

void MainWindow::createServerConnection(ConnectionThread* thread)
{
    qDebug()<<"New Connection";
    if (m_is_connected) return;
    m_is_connected = true;
    m_thread = thread;
    m_thread->SetGreetingMessage(m_username);
    connect(m_thread, &ConnectionThread::connectionReady, this, &MainWindow::onConnectionReady);
    m_thread->start();
}

void MainWindow::createClientConnection()
{
    m_thread = new ConnectionThread(m_type, m_ip, m_port, this);
    m_thread->SetGreetingMessage(m_username);
    connect(m_thread, &ConnectionThread::connectionReady, this, &MainWindow::onConnectionReady);
    m_thread->start();
}

void MainWindow::initialize()
{
    if (m_type == Const::Client) createClientConnection();

    ui->label_info->setText(tr("Waiting for connection..."));
    m_is_choosing_color = false;
    setBlock(true);
    ui->pushButton_throw->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->label_color1->setPixmap(QPixmap());
    ui->label_color2->setPixmap(QPixmap());
    if (m_type == Const::Server)
    {
        ui->label_ip1->setText(QString("%1:%2").arg(Const::GetLocalIp()).arg(m_port));
        ui->label_ip2->setText("");
        ui->groupBox_player_1->setTitle(m_username);
        ui->groupBox_player_2->setTitle("Player2");
    }
    else if (m_type == Const::Client)
    {
        ui->label_ip1->setText("");
        ui->label_ip2->setText("");
        ui->groupBox_player_1->setTitle("Player1");
        ui->groupBox_player_2->setTitle(m_username);
    }
}



void MainWindow::onConnectionReady(const QString& oppUsername)
{
    m_connection = m_thread->GetConnection();
    connect(this, &MainWindow::moveSent, m_connection, &Connection::sendMove);
    connect(this, &MainWindow::messageSent, m_connection, &Connection::sentMessage);
    connect(this, &MainWindow::disconnected, m_connection, &Connection::close);
    connect(m_connection, &Connection::gameStartReceived, this, &MainWindow::onChooseColor);
    connect(m_connection, &Connection::moveReceived, this, &MainWindow::onOpponentMove);
    connect(m_connection, &Connection::opponentThrowRecived, this, &MainWindow::onOpponentThrow);
    connect(m_connection, &QTcpSocket::disconnected, this, &MainWindow::onDisConnected);

    ui->label_status->setText("Connected");
    ui->label_status->setStyleSheet("color:green;");
    ui->pushButton_disconnnect->setEnabled(true);
    qDebug()<<"Server Ready "<<oppUsername;

    if (m_type == Const::Server)
    {
        ui->groupBox_player_2->setTitle(oppUsername);
        ui->label_ip2->setText(QString("%1:%2").arg(m_connection->peerAddress().toString()).arg(m_connection->peerPort()));
        ui->pushButton_start->setEnabled(true);
        ui->label_info->setText(tr("Press the start button to start a new game"));
    }
    else if (m_type == Const::Client)
    {
        ui->groupBox_player_1->setTitle(oppUsername);
        ui->label_ip1->setText(QString("%1:%2").arg(m_ip).arg(m_port));
        ui->label_ip2->setText(QString("%1:%2").arg(Const::GetLocalIp()).arg(m_connection->localPort()));
        ui->label_info->setText(tr("Waiting for the server to start..."));
    }
    onGameStartPrepare();
}

/*void MainWindow::onConnectionError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"ConnectionError";
}*/

void MainWindow::onDisConnected()
{
    m_is_connected = false;
    ui->label_status->setText("Disconnected");
    ui->label_status->setStyleSheet("color:red;");
    ui->pushButton_disconnnect->setEnabled(false);
    if (m_is_closing) return;
    qDebug()<<"DisConnected";
    m_thread->deleteLater();
    QMessageBox::information(this, tr("Disconnected"), tr("Connection has been disconnected!"));
    if (m_type == Const::Server) initialize();
    else if (m_type == Const::Client) qApp->exit(233);
}

void MainWindow::onGameStartPrepare()
{
    if (m_is_choosing_color) return;
    this->setBlock(true);
    ui->pushButton_throw->setEnabled(false);
    if (m_type == Const::Server)
    {
        ui->pushButton_start->setEnabled(true);
        ui->label_info->setText(tr("Press the start button to start a new game"));
    }
    else if (m_type == Const::Client)
        ui->label_info->setText(tr("Waiting for the server to start..."));
}

void MainWindow::onChooseColor()
{
    m_is_choosing_color = true;
    ui->board->Clear();
    ui->label_info->setText(tr("Waiting for choosing color..."));

    ChooseColorDialog dialog(m_username, m_type, this);
    connect(m_connection, &Connection::prepareStateReceived, &dialog, &ChooseColorDialog::onUpdateState);
    connect(m_connection, &QTcpSocket::disconnected, &dialog, &ChooseColorDialog::onDisconnected);
    connect(&dialog, &ChooseColorDialog::prepareStateChanged, m_connection, &Connection::sendPrepareState);
    if (dialog.exec() != QDialog::Accepted)
    {
        if (dialog.IsDisconnected())
            return;
        else
            this->close();
    }

    if (dialog.FirstPlayer() == m_type)
    {
        setBlock(false);
        m_player = new Player(Pieces::Black);
        ui->label_info->setText(tr("Please select a position to place the pieces"));
    }
    else
    {
        setBlock(true);
        m_player = new Player(Pieces::White);
        ui->label_info->setText(tr("Waiting for the opponent to place..."));
    }

    QPixmap pixmap(18, 18);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(dialog.FirstPlayer() == Const::Server? Qt::black : Qt::white);
    painter.drawEllipse(QPoint(9, 9), 8, 8);
    ui->label_color1->setPixmap(pixmap);

    pixmap.fill(Qt::transparent);
    painter.setBrush(dialog.FirstPlayer() == Const::Server? Qt::white : Qt::black);
    painter.drawEllipse(QPoint(9, 9), 8, 8);
    ui->label_color2->setPixmap(pixmap);

    ui->pushButton_throw->setEnabled(true);
    ui->pushButton_start->setEnabled(false);
    ui->board->SetColor(m_player->Color());
}

void MainWindow::onOpponentMove(int row, int col, Pieces::PiecesColor color)
{
    m_is_choosing_color = false;
    ui->label_info->setText(tr("Please select a position to place the pieces"));
    setBlock(false);
    ui->board->PlacePiece(row, col, (Pieces::PiecesColor)color);
}

void MainWindow::onMyMove(int row, int col, Pieces::PiecesColor color)
{
    m_is_choosing_color = false;
    ui->label_info->setText(tr("Waiting for the opponent to place..."));
    setBlock(true);
    emit moveSent(row, col, color);
}

void MainWindow::onOpponentThrow()
{
    m_is_choosing_color = false;
    QMessageBox::information(this, tr("WIN"), tr("The opponent throw the game :-)"));
    onGameStartPrepare();
}

void MainWindow::onMyThrow()
{
    m_is_choosing_color = false;
    emit messageSent("throw");
    QMessageBox::information(this, tr("LOSE"), tr("You throw the game :-("));
    onGameStartPrepare();
}

void MainWindow::on_pushButton_throw_clicked()
{
    if (QMessageBox::question(this, tr("Throw Game"), tr("Do you really want to throw the game?")) == QMessageBox::Yes)
        onMyThrow();
}

void MainWindow::on_pushButton_disconnnect_clicked()
{
    emit disconnected();
}

void MainWindow::on_pushButton_start_clicked()
{
    if (m_type == Const::Server)
    {
        emit messageSent("start");
        onChooseColor();
    }
}

void MainWindow::on_pushButton_hint_clicked()
{
    ui->board->ShowHint();
}
