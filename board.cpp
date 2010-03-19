#include <QtGui/QGraphicsView>

#include "board.h"
#include "cell.h"

Board::Board(int rows, int cols)
    : rows(rows), cols(cols),
      cellWidth(50), cellHeight(50)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cellList << new Cell(*this, row, col);
        }
    }
}

Cell* Board::cell(int row, int col)
{
    if (0 <= row && row <= rows &&
        0 <= col && col <= cols) {
        return cellList[row * cols + col];
    } else
        return 0;
}

void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Grid
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    // vertical lines
    for (int i = 0; i <= cols; i++) {
        QLineF line(QPointF(i * cellWidth, 0), QPointF(i * cellWidth, rows * cellHeight));
        painter->drawLine(line);
    }
    // horizontal lines
    for (int i = 0; i <= rows; i++) {
        QLineF line(QPointF(0, i * cellHeight), QPointF(cols * cellWidth, i * cellHeight));
        painter->drawLine(line);
    }
}

