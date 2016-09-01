#ifndef BOARD_H
#define BOARD_H

#include "const.h"
#include "pieces.h"

#include <QPoint>
#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT

public:
    Board(QWidget* parent = nullptr);

    //void PlacePiece(Pieces* pieces) { m_board[pieces->Row()][pieces->Column()] = pieces->Color(); }
    void SetBlock(bool isBlock) { m_is_block = isBlock; }
    void SetColor(Pieces::PiecesColor color) { m_color = color; }
    //void PlacePiece(Pieces* pieces);
    void PlacePiece(int row, int col, Pieces::PiecesColor color);

    void Clear();

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

    bool checkWin(int row, int col, int color);

signals:
    void gameOver();
    void piecePlaced(int row, int col, Pieces::PiecesColor color);
};

#endif // BOARD_H
