#ifndef PLAYER_H
#define PLAYER_H

#include "pieces.h"

class Player
{
public:
    Player(Pieces::PiecesColor type);

    Pieces::PiecesColor Color() const { return m_color; }

private:
    Pieces::PiecesColor m_color;
    int m_time_used;
};

#endif // PLAYER_H
