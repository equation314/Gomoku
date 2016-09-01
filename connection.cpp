#include "connection.h"

#include <QDebug>
#include <QDataStream>
#include <QThread>

Connection::Connection(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, &QTcpSocket::readyRead, this, &Connection::onReceivedData);
    connect(this, &QTcpSocket::connected, this, &Connection::greeting);
}



void Connection::sentMessage(const QString &message)
{
    qDebug()<<"seed message "<<message;
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("MESSAGE") << message;
    this->write(array);
}

void Connection::sendMove(int row, int col, Pieces::PiecesColor color)
{
    qDebug()<<"seed move";
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("MOVE");
    out << row << col << (int)color;
    this->write(array);
}

void Connection::sendPrepareState(bool isAccepted, int state0, int state1, const QString &username)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("PREPARE");
    out << isAccepted << state0 << state1 << username;
    this->write(array);
}

void Connection::greeting()
{
    qDebug()<<m_greeting_message;
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("GREETING") << m_greeting_message;
    this->write(array);
}

void Connection::onReceivedData()
{
    QByteArray array = this->readAll();
    QDataStream in(array);
    for (QString type; !in.atEnd();)
    {
        in >> type;
        qDebug()<<"Received"<<' '<<type;
        if (type == "GREETING")
        {
            QString username;
            in >> username;
            qDebug()<<username;
            emit connectionReady(username);
        }
        else if (type == "PREPARE")
        {
            bool isAccepted;
            int state0, state1;
            QString username;
            in >> isAccepted >> state0 >> state1 >> username;
            emit prepareStateReceived(isAccepted, state0, state1, username);
        }
        else if (type == "MOVE")
        {
            int row, col, color;
            in >> row >> col >> color;
            emit moveReceived(row, col, (Pieces::PiecesColor)color);
        }
        else if (type == "MESSAGE")
        {
            QString message;
            in >> message;
            qDebug()<<message;
            if (message == "start")
                emit gameStartReceived();
            else if (message == "throw")
                emit opponentThrowRecived();
        }
    }
}
