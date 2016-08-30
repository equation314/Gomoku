#ifndef BOARD_H
#define BOARD_H

#include "const.h"
#include "pieces.h"

#include <QWidget>
#include <QPoint>

class Board : public QWidget
{
public:
    Board(QWidget* parent = nullptr);

    void PlacePiece(Pieces* pieces) { m_board[pieces->Row()][pieces->Column()] = pieces->Color(); }
    void PlacePiece(int row, int col, Pieces::PiecesType color) { m_board[row][col] = color; }

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Pieces::PiecesType m_board[Const::SIZE + 1][Const::SIZE + 1];
    QPointF m_center;
    double m_cell_width;
};

#endif // BOARD_H
