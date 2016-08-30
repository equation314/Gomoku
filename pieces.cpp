#include "pieces.h"

Pieces::Pieces() :
    m_round(-1), m_color(Transpraent), m_state(None)
{

}

Pieces::Pieces(int row, int col, PiecesColor color, int round) :
    m_row(row), m_col(col), m_round(round), m_color(color), m_state(Placed)
{

}
