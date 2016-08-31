#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "const.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(const QString& username, Const::NetworkIdentity type, const QString& ip, int port, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString m_username;
    Const::NetworkIdentity m_type;
    QString m_ip;
    int m_port;

    Player* m_player;
    bool m_is_block, m_is_closing;

    QTcpServer* m_server;
    QTcpSocket* m_socket;

    void createConnection();
    void startGame();
    void setBlock(bool isBlock);

private slots:
    void onNewConnection();
    void onConnectionError(QAbstractSocket::SocketError socketError);
    void onConnected();
    void onDisConnected();
    void onReceivedData();

    void onPiecePlaced(int row, int col, Pieces::PiecesColor color);
};

#endif // MAINWINDOW_H
