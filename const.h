#ifndef CONST_H
#define CONST_H

#include <QColor>

namespace Const
{

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

}

#endif // CONST_H
