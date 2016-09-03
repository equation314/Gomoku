#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "const.h"
#include "server.h"
#include "connectionthread.h"

#include <QTimer>
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
    Const::HostType m_type, m_current_player;
    QString m_ip;
    int m_port;

    Server* m_server;
    Connection* m_connection;
    ConnectionThread* m_thread;

    QTimer m_timer;
    int m_time_left, m_my_tot_time, m_opp_tot_time;
    bool m_can_back;
    bool m_is_block, m_is_started, m_is_closing, m_is_choosing_color, m_is_connected;

    void setBlock(bool isBlock);
    void initialize();
    void createServerConnection(ConnectionThread *thread);
    void createClientConnection();
    void nextMove();

private slots:
    void onTimeOut();

    void onConnectionReady(const QString& oppUsername);
    void onDisConnected();

    void onGameStartPrepare();
    void onChooseColor();
    void onPause();
    void onContinue();

    void onMyMove(int row, int col, Pieces::PiecesColor color);
    void onOpponentMove(int row, int col, Pieces::PiecesColor color);
    void onOpponentBackRequest();
    void onOpponentThrow();

    void on_pushButton_start_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_hint_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_throw_clicked();
    void on_pushButton_disconnect_clicked();

    void on_action_about_triggered();

    void on_action_exit_triggered();

signals:
    void disconnected();
    void gameStarted();
    void messageSent(const QString& message);
    void moveSent(int row, int col, Pieces::PiecesColor color);
};

#endif // MAINWINDOW_H
