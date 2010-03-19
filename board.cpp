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
    if (0 <= row && row < rows &&
        0 <= col && col < cols) {
        return cellList[row * cols + col];
    } else
        return 0;
}

void Board::setCellStates()
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int state = (row + col) % 3;
            cell(row, col)->setState(state);
        }
    }
}

void Board::updateCellStates()
{
    Cell& cell = *this->cell(qrand() % rows, qrand() % cols);
    cell.setState((cell.state + 1) % 3);
    update();
}

void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // TODO is there a polyline type?

    // Grid
    painter->setPen(QPen(Qt::black, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    // dividers
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Cell& cell = *this->cell(row, col);
            QPointF topLeft = cell.topLeft();
            switch (cell.state) {
            case Cell::Square:
                break;
            case Cell::BendDexter:
                painter->drawLine(QLineF(topLeft, topLeft + QPointF(cellWidth, cellHeight)));
                break;
            case Cell::BendSinister:
                painter->drawLine(QLineF(topLeft + QPointF(0, cellHeight), topLeft + QPointF(cellWidth, 0)));
                break;
            }
        }
    }

    // Dual
    painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Cell& cell = *this->cell(row, col);
            if (cell.centerCount() > 1) {
                // could just as well use Left and Right
                QLineF line(cell.center(cell.Up), cell.center(cell.Down));
                painter->drawLine(line);
            }
            if (row+1 < rows) {
                painter->drawLine(QLineF(cell.center(cell.Down), this->cell(row+1, col)->center(cell.Up)));
            }
            if (col+1 < cols) {
                painter->drawLine(QLineF(cell.center(cell.Right), this->cell(row, col+1)->center(cell.Left)));
            }
        }
    }
}

