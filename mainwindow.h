#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "const.h"
#include "player.h"
#include "server.h"
#include "connectionthread.h"

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(const QString& username, Const::HostType type, const QString& ip, int port, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString m_username, m_opp_username;
    Const::HostType m_type;
    QString m_ip;
    int m_port;

    Player* m_player;
    bool m_is_block, m_is_closing, m_is_choosing_color, m_is_connected;

    Server* m_server;
    Connection* m_connection;
    ConnectionThread* m_thread;

    void setBlock(bool isBlock);
    void initialize();
    void createServerConnection(ConnectionThread *thread);
    void createClientConnection();

private slots:
    void onConnectionReady(const QString& oppUsername);
    void onDisConnected();
    //void onConnectionError(QAbstractSocket::SocketError socketError);

    void onChooseColor();
    void onGameStartPrepare();
    void onMyMove(int row, int col, Pieces::PiecesColor color);
    void onOpponentMove(int row, int col, Pieces::PiecesColor color);
    void onMyThrow();
    void onOpponentThrow();

    void on_pushButton_disconnnect_clicked();
    void on_pushButton_throw_clicked();
    void on_pushButton_start_clicked();

    void on_pushButton_hint_clicked();

signals:
    void disconnected();
    void gameStarted();
    void messageSent(const QString& message);
    void moveSent(int row, int col, Pieces::PiecesColor color);
};

#endif // MAINWINDOW_H
