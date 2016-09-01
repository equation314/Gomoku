#include "const.h"
#include "board.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

using namespace std;

Board::Board(QWidget* parent) :
    QWidget(parent),
    m_is_block(true), m_round(0)
{
    this->setMouseTracking(true);
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++) m_bomb[i][j] = 0;
    connect(&m_bomb_timer, &QTimer::timeout, this, [this]()
    {
        qDebug()<<"fuck";
        for (int i = 0; i <= Const::SIZE; i++)
            for (int j = 0; j <= Const::SIZE; j++) m_bomb[i][j] = 0;
        this->update();
    });
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
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(m_center);
    int halfSize = Const::SIZE / 2;
    double halfWidth = Const::SIZE * m_cell_width / 2;
    double frameWidth = (Const::SIZE + 2) * m_cell_width;

    QPen pen(Qt::black);
    pen.setWidth(2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);
    painter.setBrush(QColor(255, 255, 128));
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
        {
            QPointF center((i - halfSize) * m_cell_width , (j - halfSize) * m_cell_width);
            if (m_bomb[i][j])
                painter.drawPixmap(center.x() - r, center.y() - r, r * 2, r * 2, QPixmap(":/icon/bomb.png"));
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
        if (j - i >= 5) return true;
    }
    for (int i = 0, j; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; j <= Const::SIZE && m_board[row][j].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = j - row + col, 0 <= k && k <= Const::SIZE && m_board[j][k].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = row + col - j, 0 <= k && k <= Const::SIZE && m_board[j][k].Color() == color; j++);
        if (j - i >= 5) return true;
    }
    return false;
}

bool Board::hasBomb(int x, int y)
{
    static const int dir[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
                                 // --      |       \        /
    if (m_board[x][y].Color() != Pieces::Transpraent) return false;
    Pieces::PiecesColor oppColor = m_color == Pieces::Black ? Pieces::White : Pieces::Black;
    m_board[x][y].SetColor(oppColor);

    int n3 = 0, n4 = 0;
    for (int i = 0; i < Const::SIZE; i++)
        for (int j = 0; j < Const::SIZE; j++)
            if (m_board[i][j].Color() == oppColor)
            {
                for (int d = 0; d < 4; d++)
                {
                    int ii = i, jj = j, k;
                    for (k = 0; k < 4 && isOnBoard(ii, jj) && m_board[ii][jj].Color() == oppColor; k++, ii += dir[d][0], jj += dir[d][1]);
                    if (k == 3 &&  isAvailable(ii, jj) && isAvailable(i - dir[d][0], j - dir[d][1]))  n3++;
                    if (k == 4 && (isAvailable(ii, jj) || isAvailable(i - dir[d][0], j - dir[d][1]))) n4++;

                    if (n3 >= 2 || (n3 && n4))
                    {
                        m_board[x][y].SetColor(Pieces::Transpraent);
                        return true;
                    }
                }
            }
    m_board[x][y].SetColor(Pieces::Transpraent);
    return false;
}

void Board::Clear()
{
    m_round = 0;
    for (int i = 0 ; i <= Const::SIZE; i++)
        for (int j = 0 ; j <= Const::SIZE; j++) m_board[i][j] = Pieces();
    this->update();
}

void Board::PlacePiece(int row, int col, Pieces::PiecesColor color)
{
    m_board[row][col] = Pieces(row, col, color, ++m_round);
    this->update();
    if (checkWin(row, col, color))
    {
        if (color == m_color)
            QMessageBox::information(this, tr("WIN!"), tr("Congratulation! You win the game :-)"));
        else
            QMessageBox::information(this, tr("LOSE"), tr("You lose the game :-("));
        emit gameOver();
    }
    else if (m_round == Const::SIZE * Const::SIZE)
    {
        QMessageBox::information(this, tr("Draw"), tr("2333333333..."));
        emit gameOver();
    }
}

void Board::ShowHint()
{
    m_bomb_timer.setSingleShot(true);
    m_bomb_timer.start(2000);
    qDebug()<<m_bomb_timer.isActive();
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            m_bomb[i][j] = hasBomb(i, j);
    this->update();
}

