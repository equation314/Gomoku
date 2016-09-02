#ifndef CONNECTION_H
#define CONNECTION_H

#include "pieces.h"

#include <QTcpSocket>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent = nullptr);

    void SetGreetingMessage(const QString& message) { m_greeting_message = message; }

public slots:
    /*!!! Cannot invoke directly in different thread !!!*/
    void sentMessage(const QString &message);
    void sendMove(int row, int col, Pieces::PiecesColor color);
    void sendPrepareState(bool isAccepted, int state0, int state1, const QString& username);
    void greeting();

private:
    QString m_greeting_message;

private slots:
    void onReceivedData();

signals:
    void connectionReady(const QString& username);
    void gameStartReceived();
    void prepareStateReceived(bool isAccepted, int state0, int state1, const QString& username);
    void moveReceived(int row, int col, Pieces::PiecesColor color);
    void opponentBackRequest();
    void backAllowed();
    void backDisallowed();
    void opponentThrowRecived();
};

#endif // CONNECTION_H
