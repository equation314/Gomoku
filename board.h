#ifndef BOARD_H
#define BOARD_H

#include "const.h"
#include "pieces.h"

#include <QPoint>
#include <QTimer>
#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT

public:
    Board(QWidget* parent = nullptr);

    // Setter member functions
    void SetBlock(bool isBlock) { m_is_block = isBlock; }
    void SetColor(Pieces::PiecesColor color) { m_color = color; }

    void Clear();
    void PlacePiece(int row, int col, Pieces::PiecesColor color);
    void ShowHint();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    Pieces m_board[Const::SIZE + 1][Const::SIZE + 1];
    QPointF m_center;
    Pieces::PiecesColor m_color;
    double m_cell_width;
    bool m_is_block;
    int m_round;

    QTimer m_bomb_timer;
    bool m_bomb[Const::SIZE + 1][Const::SIZE + 1];

    bool isOnBoard(int x, int y)
    {
        return 0 <= x && x <= Const::SIZE && 0 <= y && y <= Const::SIZE;
    }
    bool isAvailable(int x, int y)
    {
        return isOnBoard(x, y) && m_board[x][y].Color() == Pieces::Transpraent;
    }
    bool checkWin(int row, int col, int color);
    bool hasBomb(int x, int y);

signals:
    void gameOver();
    void piecePlaced(int row, int col, Pieces::PiecesColor color);
};

#endif // BOARD_H
