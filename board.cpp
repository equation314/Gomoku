#include "const.h"
#include "board.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

using namespace std;

Board::Board(QWidget* parent) :
    QWidget(parent),
    m_is_block(false), m_round(0)
{
    this->setMouseTracking(true);
}

void Board::resizeEvent(QResizeEvent* event)
{
    m_cell_width = 1.0 * min(this->width(), this->height()) / (Const::SIZE + 2) * 0.95;
    m_center = QPointF(this->width() / 2, this->height() / 2);
    QWidget::resizeEvent(event);
}

void Board::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.translate(m_center);
    int halfSize = Const::SIZE / 2;
    double halfWidth = Const::SIZE * m_cell_width / 2;
    double frameWidth = (Const::SIZE + 2) * m_cell_width;

    QPen pen(Qt::black);
    pen.setWidth(2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-frameWidth / 2, -frameWidth / 2, frameWidth, frameWidth);

    painter.setPen(Qt::black);
    for (int i = -halfSize; i <= halfSize; i++)
    {
        painter.drawLine(-halfWidth, i * m_cell_width, halfWidth, i * m_cell_width);
        painter.drawLine(i * m_cell_width, -halfWidth, i * m_cell_width, halfWidth);
    }

    double r = m_cell_width / 5;
    painter.setBrush(Qt::black);
    for (int i = 0; i < 5; i++)
        painter.drawEllipse((Const::POINT[i][0] - halfSize) * m_cell_width - r / 2, (Const::POINT[i][1] - halfSize) * m_cell_width - r / 2, r, r);

    r = m_cell_width / 2;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].State() != Pieces::None)
            {
                Pieces piece = m_board[i][j];
                QColor color = Const::PIECES_COLOR[piece.Color()];
                if (piece.State() == Pieces::Hover)
                {
                    color = Const::PIECES_COLOR[m_color];
                    color.setAlpha(150);
                }
                painter.setBrush(color);
                painter.setPen(Qt::black);
                QPointF center((i - halfSize) * m_cell_width , (j - halfSize) * m_cell_width);
                painter.drawEllipse(center, r, r);
                if (piece.Round() == m_round && piece.State() == Pieces::Placed)
                {
                    pen.setColor(Qt::red);
                    pen.setStyle(Qt::DashLine);
                    painter.setPen(pen);
                    painter.setBrush(Qt::transparent);
                    painter.drawRect(center.x() - r, center.y() - r, r * 2, r * 2);
                }
            }
}

void Board::mouseMoveEvent(QMouseEvent* event)
{
    if (m_is_block) return;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].State() != Pieces::Placed)
            {
                int halfSize = Const::SIZE / 2, r = m_cell_width / 2;
                QPointF center = m_center + QPointF((i - halfSize) * m_cell_width, (j - halfSize) * m_cell_width);
                if (Const::Sqr(center.x() - event->x()) + Const::Sqr(center.y() - event->y()) >= r * r)
                    m_board[i][j].SetState(Pieces::None);
                else
                    m_board[i][j].SetState(Pieces::Hover);
            }
    this->update();
    return;
}

void Board::mousePressEvent(QMouseEvent* event)
{
    if (m_is_block || event->button() != Qt::LeftButton) return;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].State() == Pieces::Hover)
            {
                int halfSize = Const::SIZE / 2, r = m_cell_width / 2;
                QPointF center = m_center + QPointF((i - halfSize) * m_cell_width, (j - halfSize) * m_cell_width);
                if (Const::Sqr(center.x() - event->x()) + Const::Sqr(center.y() - event->y()) >= r * r) continue;
                if (event->button() == Qt::LeftButton)
                {
                    emit piecePlaced(i, j, m_color);
                    PlacePiece(i, j, m_color);
                    return;
                }
            }
}

bool Board::checkWin(int row, int col, int color)
{
    for (int i = 0, j; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; j <= Const::SIZE && m_board[j][col].Color() == color; j++);
        qDebug()<<i<<' '<<j;
        if (j - i >= 5) return true;
    }
    qDebug()<<"1";
    for (int i = 0, j; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; j <= Const::SIZE && m_board[row][j].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    qDebug()<<"2";
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = j - row + col, 0 <= k && k <= Const::SIZE && m_board[j][k].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    qDebug()<<"3";
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = row + col - j, 0 <= k && k <= Const::SIZE && m_board[j][k].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    qDebug()<<"4";
    return false;
}

void Board::PlacePiece(int row, int col, Pieces::PiecesColor color)
{
    m_board[row][col] = Pieces(row, col, color, ++m_round);
    this->update();
    if (checkWin(row, col, color))
    {
        if (color == m_color)
            QMessageBox::information(this, tr("Congratulation!"), tr("You win the game :-)"));
        else
            QMessageBox::information(this, tr("Game Over"), tr("You lose the game :-("));
        emit restart();
        Restart();
    }
    else if (m_round == Const::SIZE * Const::SIZE)
    {
        QMessageBox::information(this, tr("Draw"), tr("2333333333..."));
        emit restart();
        Restart();
    }
}

void Board::Restart()
{
    m_round = 0;
    for (int i = 0 ; i <= Const::SIZE; i++)
        for (int j = 0 ; j <= Const::SIZE; j++) m_board[i][j] = Pieces();
    this->update();
}


