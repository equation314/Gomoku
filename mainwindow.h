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

    explicit MainWindow(Const::NetworkIdentity type, const QString& ip, int port, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Const::NetworkIdentity m_type;
    QString m_ip;
    int m_port;

    Player* m_player;
    bool m_is_block;

    QTcpServer* m_server;
    QTcpSocket* m_socket;

    void createConnection();

private slots:
    void onNewConnection();
    void onConnectionError(QAbstractSocket::SocketError socketError);
    void onConnected();
    void onDisConnected();
    void onReceivedData();

    void onPiecePlaced(int row, int col, Pieces::PiecesColor color);
};

#endif // MAINWINDOW_H
