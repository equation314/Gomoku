#include "pieces.h"

Pieces::Pieces() :
    m_state(None)
{

}

Pieces::Pieces(int row, int col, PiecesColor color, int round) :
    m_row(row), m_col(col), m_color(color), m_round(round), m_state(Placed)
{

}
