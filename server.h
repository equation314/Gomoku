#ifndef SERVER_H
#define SERVER_H

#include "connectionthread.h"

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QString ip, int port, QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newConnection(ConnectionThread *thread);
};

#endif // SERVER_H
