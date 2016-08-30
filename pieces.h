#ifndef PIECES_H
#define PIECES_H


class Pieces
{
public:
    enum PiecesType
    {
        None,
        White,
        Black
    };

    Pieces();

    int Row() const { return m_row; }
    int Column() const { return m_col; }
    PiecesType Color() const { return m_type; }

private:
    int m_row, m_col;
    PiecesType m_type;
};

#endif // PIECES_H
