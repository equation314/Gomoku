#ifndef PIECES_H
#define PIECES_H


class Pieces
{
public:
    enum PiecesColor
    {
        Transpraent,
        White,
        Black
    };
    enum PiecesState
    {
        None,
        Hover,
        Placed
    };

    explicit Pieces();
    explicit Pieces(int row, int col, PiecesColor color, int round);

    // Getter member functions
    int Row() const { return m_row; }
    int Column() const { return m_col; }
    int Round() const { return m_round; }
    PiecesColor Color() const { return m_color; }
    PiecesState State() const { return m_state; }

    // Setter member functions
    void SetColor(PiecesColor color) { m_color = color; }
    void SetState(PiecesState state) { m_state = state; }

private:
    int m_row, m_col, m_round;
    PiecesColor m_color;
    PiecesState m_state;
};

#endif // PIECES_H
