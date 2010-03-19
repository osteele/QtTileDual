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
    // Grid
    {
        QPainterPath path;
        // vertical lines
        for (int col = 0; col <= cols; col++) {
            //QLineF line(QPointF(col * cellWidth, 0), QPointF(col * cellWidth, rows * cellHeight));
            //painter->drawLine(line);
            path.moveTo(col * cellWidth, 0);
            path.lineTo(col * cellWidth, rows * cellHeight);
        }
        // horizontal lines
        for (int row = 0; row <= rows; row++) {
            path.moveTo(0, row * cellHeight);
            path.lineTo(cols * cellWidth, row * cellHeight);
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
                    path.moveTo(topLeft);
                    path.lineTo(topLeft + QPointF(cellWidth, cellHeight));
                    break;
                case Cell::BendSinister:
                    path.moveTo(topLeft + QPointF(0, cellHeight));
                    path.lineTo(topLeft + QPointF(cellWidth, 0));
                    break;
                }
            }
        }
        painter->setPen(QPen(Qt::black, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(path);
    }

    // Dual
    {
        QPainterPath path;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Cell& cell = *this->cell(row, col);
                if (cell.centerCount() > 1) {
                    // could just as well use Left and Right
                    path.moveTo(cell.center(cell.Up));
                    path.lineTo(cell.center(cell.Down));
                }
                if (row+1 < rows) {
                    path.moveTo(cell.center(cell.Down));
                    path.lineTo(this->cell(row+1, col)->center(cell.Up));
                }
                if (col+1 < cols) {
                    path.moveTo(cell.center(cell.Right));
                    path.lineTo(this->cell(row, col+1)->center(cell.Left));
                }
            }
            painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPath(path);
        }
    }
}

