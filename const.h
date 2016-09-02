#ifndef CONST_H
#define CONST_H

#include <QColor>
#include <QHostInfo>
#include <QNetworkInterface>

namespace Const
{

enum HostType
{
    Server,
    Client
};

const int TIME_LIMIT = 30;
const int SIZE = 14;
const int POINT[5][2] =
{
    {7,  7 },
    {3,  3 },
    {3,  11},
    {11, 3 },
    {11, 11}
};

const QColor PIECES_COLOR[3] =
{
    Qt::transparent,
    Qt::white,
    Qt::black
};

inline double Sqr(double x) { return x * x; }

inline QString GetLocalIp()
{
#ifdef Q_OS_WIN
    auto list = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
#else
    auto list = QNetworkInterface::allAddresses();
#endif
    for (auto i : list)
        if (i != QHostAddress::LocalHost && i.protocol() == QAbstractSocket::IPv4Protocol)
            return i.toString();
    return "";
}

}

#endif // CONST_H
