#ifndef PLAYER_H
#define PLAYER_H

#include "pieces.h"

class Player
{
public:
    Player(Pieces::PiecesColor type);

    Pieces::PiecesColor Type() const { return m_type;
                                    }
private:
    Pieces::PiecesColor m_type;
    int m_time_used;
};

#endif // PLAYER_H
