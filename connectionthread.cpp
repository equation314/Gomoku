#include "connectionthread.h"

#include <QHostAddress>

ConnectionThread::ConnectionThread(Const::HostType type, QObject* parent) :
    QThread(parent),
    m_type(type)
{

}

ConnectionThread::ConnectionThread(Const::HostType type, const QString& ip, int port, QObject* parent) :
    QThread(parent),
    m_type(type), m_ip(ip), m_port(port)
{

}

ConnectionThread::~ConnectionThread()
{
    this->quit();
    this->wait();
    m_connection->deleteLater();
}

void ConnectionThread::run()
{
    m_connection = new Connection;
    m_connection->SetGreetingMessage(m_greeting_massage);
    connect(m_connection, &Connection::connectionReady, this, &ConnectionThread::connectionReady);
    connect(m_connection, &Connection::disconnected, this, &QThread::quit);

    if (m_type == Const::Server)
    {
        m_connection->setSocketDescriptor(m_socketDescriptor);
        m_connection->greeting();
    }
    else
        m_connection->connectToHost(QHostAddress(m_ip), m_port);

    this->exec();
}
